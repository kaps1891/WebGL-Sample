# WebGL-Sample
A basic app for drawing a triangle on Browser using GL commands.


# To compile, use this:

emcc -g --bind triangle.cpp -O0 -s USE_GLFW=3 -s FULL_ES2=1 -s MAX_WEBGL_VERSION=2 -s OFFSCREENCANVAS_SUPPORT=1 -s FULL_ES2=1 -pthread -s PTHREAD_POOL_SIZE=4 -s 'EXPORTED_RUNTIME_METHODS=['cwrap', 'GL', 'findCanvasEventTarget', 'ccall']'

# On a terminal:

1. npm install -g serve

2. serve -s . --listen 8080 --ssl-cert ./localhost.adobe.com.pem --ssl-key ./localhost.adobe.com-key.pem

# Then, on the browser:

http://localhost:8080/
