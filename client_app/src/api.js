export async function getStatus(ip) {
    try {
      const response = await fetch(`http://${ip}/status`);
      if (!response.ok) throw new Error("Status fetch failed");
      return await response.json();
    } catch (error) {
      return { state: 'OFFLINE', lives: '-' };
    }
  }
  
  export async function startGame(ip) {
    try {
      await fetch(`http://${ip}/start`);
    } catch (error) {
      console.error("Failed to start game", error);
    }
  }
  
  export async function resetGame(ip) {
    try {
      await fetch(`http://${ip}/reset`);
    } catch (error) {
      console.error("Failed to reset game", error);
    }
  }
  