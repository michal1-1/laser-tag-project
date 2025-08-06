import { useEffect, useState, useCallback, useRef } from 'react';
import './App.css';
import { getStatus, startGame, resetGame } from './api';
const players = [
  { id: 1, ip: '192.168.76.198' },
  { id: 2, ip: '192.168.76.199' }
];
function GameTimer({ timeRemaining, gameStartTime, gameDuration = 120000 }) {
  const [currentTime, setCurrentTime] = useState(Date.now());
  useEffect(() => {
    const interval = setInterval(() => {
      setCurrentTime(Date.now());
    }, 1000);
    return () => clearInterval(interval);
  }, []);
  const formatTime = (ms) => {
    const totalSeconds = Math.ceil(ms / 1000);
    const minutes = Math.floor(totalSeconds / 60);
    const seconds = totalSeconds % 60;
    return `${minutes}:${seconds.toString().padStart(2, '0')}`;
  };
  if (timeRemaining !== undefined && timeRemaining !== null) {
    if (timeRemaining <= 0) return null;
    return (
      <div className="game-timer">
        <span className="timer-icon">⏱️</span>
        <span className="timer-value">{formatTime(timeRemaining)}</span>
      </div>
    );
  }
  if (gameStartTime) {
    const elapsed = currentTime - gameStartTime;
    const remainingTime = Math.max(0, gameDuration - elapsed);
    if (remainingTime <= 0) return null;
    return (
      <div className="game-timer">
        <span className="timer-icon">⏱️</span>
        <span className="timer-value">{formatTime(remainingTime)}</span>
      </div>
    );
  }

  return null;
}

function WinnerAnnouncement({ winner, onClose }) {
  if (!winner) return null;

  return (
    <div className="winner-overlay">
      <div className="winner-card">
        {winner.type === 'tie' ? (
          <>
            <h2 className="winner-title tie">🤝 TIE GAME!</h2>
            <p className="winner-text">It's a Draw!</p>
            <p className="winner-stats">{winner.lives}</p>
          </>
        ) : (
          <>
            <h2 className="winner-title">🏆 WINNER!</h2>
            <p className="winner-text">Player {winner.id} Wins!</p>
            <p className="winner-stats">{winner.lives} lives remaining</p>
          </>
        )}
        <button className="winner-close" onClick={onClose}>
          ✨ Continue
        </button>
      </div>
    </div>
  );
}

function GameStatus({ playerStatuses, onStartAll }) {
  const allWaiting = Object.values(playerStatuses).every(s => s.state === 'WAITING');
  const anyPlaying = Object.values(playerStatuses).some(s => s.state === 'PLAYING' || s.state === 'COUNTDOWN');
  
  if (Object.keys(playerStatuses).length < 2) return null;
  
  return (
    <div style={{
      background: 'rgba(0, 255, 255, 0.15)',
      border: '2px solid rgba(0, 255, 255, 0.4)',
      borderRadius: '15px',
      padding: '2rem',
      marginBottom: '2rem',
      textAlign: 'center',
      maxWidth: '600px'
    }}>
      <h3 style={{
        color: '#00ffff',
        fontSize: '1.5rem',
        marginBottom: '1rem',
        fontFamily: "'Orbitron', monospace"
      }}>🎮 Game Control</h3>
      
      {allWaiting && (
        <>
          <p style={{ color: '#00ff88', marginBottom: '1rem' }}>
            ✅ All players ready! Click to start synchronized game
          </p>
          <button
            style={{
              background: 'linear-gradient(135deg, #00ff88, #00cc6a)',
              color: '#000',
              border: 'none',
              padding: '1rem 3rem',
              borderRadius: '50px',
              fontSize: '1.3rem',
              fontWeight: 700,
              cursor: 'pointer',
              textTransform: 'uppercase',
              letterSpacing: '2px',
              boxShadow: '0 8px 25px rgba(0, 255, 136, 0.5)'
            }}
            onClick={onStartAll}
          >
            🚀 START ALL PLAYERS
          </button>
        </>
      )}
      
      {anyPlaying && (
        <p style={{ color: '#ffff00' }}>
          ⚡ Game in progress...
        </p>
      )}
      
      {!allWaiting && !anyPlaying && (
        <p style={{ color: '#ff4757' }}>
          ⏳ Waiting for all players to be ready...
        </p>
      )}
    </div>
  );
}

function PlayerCard({ player, onStatusChange }) {
  const [status, setStatus] = useState({ state: 'LOADING', lives: '-' });
  const [isLoading, setIsLoading] = useState(true);
  const [gameStartTime, setGameStartTime] = useState(null);
  const gameOverTimeoutRef = useRef(null);
  const lastStatusRef = useRef(null);

  const updateStatus = useCallback((newStatus) => {
    const statusString = JSON.stringify(newStatus);
    if (statusString !== lastStatusRef.current) {
      lastStatusRef.current = statusString;
      
      if (newStatus.state === 'PLAYING') {
        if (!gameStartTime && !newStatus.time_remaining) {
          setGameStartTime(Date.now());
        }
      } else {
        setGameStartTime(null);
      }
      
      setStatus(newStatus);
      if (onStatusChange) {
        onStatusChange(player.id, newStatus);
      }
    }
  }, [player.id, onStatusChange, gameStartTime]);

  useEffect(() => {
    const fetchStatus = async () => {
      try {
        setIsLoading(true);
        const data = await getStatus(player.ip);
        
        if (data.state === 'GAME_OVER' && !gameOverTimeoutRef.current) {
          console.log(`Player ${player.id} reached GAME_OVER - will reset in 3 seconds`);
          
          gameOverTimeoutRef.current = setTimeout(async () => {
            try {
              await resetGame(player.ip);
              console.log(`Player ${player.id} reset to WAITING`);
              gameOverTimeoutRef.current = null;
            } catch (error) {
              console.error(`Failed to reset player ${player.id}:`, error);
              gameOverTimeoutRef.current = null;
            }
          }, 3000);
        }
        if (data.state !== 'GAME_OVER' && gameOverTimeoutRef.current) {
          clearTimeout(gameOverTimeoutRef.current);
          gameOverTimeoutRef.current = null;
        }
        
        updateStatus(data);
      } catch (error) {
        console.error('Failed to fetch status:', error);
        updateStatus({ state: 'OFFLINE', lives: '-' });
      } finally {
        setIsLoading(false);
      }
    };

    fetchStatus();
    const interval = setInterval(fetchStatus, 1000);
    
    return () => {
      clearInterval(interval);
      if (gameOverTimeoutRef.current) {
        clearTimeout(gameOverTimeoutRef.current);
      }
    };
  }, [player.ip, updateStatus]);

  const handleReset = async () => {
    try {
      await resetGame(player.ip);
      // Clear any pending GAME_OVER timeout
      if (gameOverTimeoutRef.current) {
        clearTimeout(gameOverTimeoutRef.current);
        gameOverTimeoutRef.current = null;
      }
    } catch (error) {
      console.error('Failed to reset game:', error);
    }
  };

  const getStateColor = (state) => {
    switch (state) {
      case 'WAITING': return '#00ff88';
      case 'COUNTDOWN': return '#ffff00';
      case 'PLAYING': return '#00ffff';
      case 'GAME_OVER': return '#ff4757';
      default: return '#a0a0a0';
    }
  };

  const getReadyIcon = (state) => {
    switch (state) {
      case 'WAITING': return '✅';
      case 'COUNTDOWN': return '⏳';
      case 'PLAYING': return '⚡';
      case 'GAME_OVER': return '💀';
      default: return '❓';
    }
  };
  return (
    <div className={`card ${isLoading ? 'loading' : ''} ${status.state.toLowerCase()}`}>
      <h2>🎮 Player {player.id} {getReadyIcon(status.state)}</h2>
      
      {status.state === 'PLAYING' && (
        <GameTimer 
          timeRemaining={status.time_remaining}
          gameStartTime={gameStartTime}
          gameDuration={120000}
        />
      )}    
      <div className="status">
        <div className="status-item">
          <span className="status-icon">🕹️</span>
          <span className="status-label">State:</span>
          <span 
            className={`status-value state ${status.state.toLowerCase()}`}
            style={{ color: getStateColor(status.state) }}
          >
            {status.state}
          </span>
        </div>
        <div className="status-item">
          <span className="status-icon">❤️</span>
          <span className="status-label">Lives:</span>
          <span className="status-value lives">{status.lives}</span>
        </div>
        {status.state === 'PLAYING' && status.cooldown_remaining > 0 && (
          <div className="status-item">
            <span className="status-icon">🛡️</span>
            <span className="status-label">Cooldown:</span>
            <span className="status-value cooldown">
              {Math.ceil(status.cooldown_remaining / 1000)}s
            </span>
          </div>
        )}
      </div>
      
      <div className="actions">
        <button className="reset" onClick={handleReset}>
          ⟳ Reset
        </button>
      </div>
    </div>
  );
}

export default function App() {
  const [playerStatuses, setPlayerStatuses] = useState({});
  const [winner, setWinner] = useState(null);
  const winnerShownRef = useRef(new Set());

  const handleStatusChange = useCallback((playerId, status) => {
    setPlayerStatuses(prev => {
      const newStatuses = { ...prev, [playerId]: status };
      return newStatuses;
    });
  }, []);

  const handleStartAll = async () => {
    try {
      console.log('Starting all players simultaneously...');
      const startPromises = players.map(player => startGame(player.ip));
      await Promise.all(startPromises);
      console.log('All players started!');
    } catch (error) {
      console.error('Failed to start some players:', error);
    }
  };

  useEffect(() => {
    const statuses = Object.values(playerStatuses);
    if (statuses.length < 2) return; // Wait for both players
    
    const playingPlayers = statuses.filter(s => s.state === 'PLAYING');
    const gameOverPlayers = statuses.filter(s => s.state === 'GAME_OVER');
    const waitingPlayers = statuses.filter(s => s.state === 'WAITING');
    
    if (playingPlayers.length === 1 && gameOverPlayers.length >= 1) {
      const winnerId = playingPlayers[0].player_id;
      const winnerKey = `winner_${winnerId}_${playingPlayers[0].lives}`;
      
      if (!winnerShownRef.current.has(winnerKey)) {
        const winnerData = {
          id: winnerId,
          lives: playingPlayers[0].lives,
          type: 'winner'
        };
        setWinner(winnerData);
        winnerShownRef.current.add(winnerKey);
      }
    }
    
    if (gameOverPlayers.length >= 2 && playingPlayers.length === 0) {
      const tieKey = `tie_${gameOverPlayers.map(p => p.lives).join('_')}`;
      
      if (!winnerShownRef.current.has(tieKey)) {
        const winnerData = {
          id: null,
          lives: gameOverPlayers.map(p => `Player ${p.player_id}: ${p.lives}`).join(', '),
          type: 'tie'
        };
        setWinner(winnerData);
        winnerShownRef.current.add(tieKey);
      }
    }
    
    if (waitingPlayers.length >= 1 && gameOverPlayers.length === 0) {
      winnerShownRef.current.clear();
    }
  }, [playerStatuses]);
  const handleWinnerClose = async () => {
    try {
      const resetPromises = players.map(player => resetGame(player.ip));
      await Promise.all(resetPromises);
      console.log('All players reset to WAITING after winner screen');
    } catch (error) {
      console.error('Failed to reset some players:', error);
    }
    
    setWinner(null);
    winnerShownRef.current.clear(); 
  };

  return (
    <div className="app-container">
      <h1>⚡ Laser Tag Dashboard</h1>
      
      <GameStatus 
        playerStatuses={playerStatuses}
        onStartAll={handleStartAll}
      />
      
      <div className="dashboard">
        {players.map(player => (
          <PlayerCard 
            key={player.id} 
            player={player}
            onStatusChange={handleStatusChange}
          />
        ))}
      </div>
      
      <WinnerAnnouncement 
        winner={winner} 
        onClose={handleWinnerClose}
      />
    </div>
  );
}