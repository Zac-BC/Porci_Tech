import asyncio
import websockets

async def handle_connection(websocket, path):
    async for message in websocket:
        print(f"Mensaje recibido: {message}")
        await websocket.send(f"Mensaje recibido: {message}")

async def main():
    start_server = await websockets.serve(handle_connection, "localhost", 8765)
    await start_server.wait_closed()

asyncio.run(main())