import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'

// https://vite.dev/config/
export default defineConfig({
  plugins: [react()],
  server: {
    proxy: {
      '/start': 'http://192.168.76.198',
      '/reset': 'http://192.168.76.198',
      '/status': 'http://192.168.76.198'
    }
  }
})


