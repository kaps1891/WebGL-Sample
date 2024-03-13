# WebGL-Sample

A basic app for drawing a triangle on Browser using GL commands.

## Compilation Instructions

To compile the WebGL-Sample, follow these steps:

```bash
emcc -g --bind triangle.cpp -O0 -s USE_GLFW=3 -s FULL_ES2=1 -s MAX_WEBGL_VERSION=2 -s OFFSCREENCANVAS_SUPPORT=1 -s FULL_ES2=1 -pthread -s PTHREAD_POOL_SIZE=4 -s 'EXPORTED_RUNTIME_METHODS=['cwrap', 'GL', 'findCanvasEventTarget', 'ccall']'
```

## On a terminal:

```bash
npm install -g serve

serve -s . --listen 8080 --ssl-cert ./localhost.adobe.com.pem --ssl-key ./localhost.adobe.com-key.pem
```

# Then, on the browser:

http://localhost:8080/
