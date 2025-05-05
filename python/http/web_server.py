from http.server import HTTPServer, BaseHTTPRequestHandler
import ssl

class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/plain')
        self.end_headers()
        self.wfile.write(b"Hello, this is a secure response from Python HTTPS server!")

    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)
        print("Received POST data:", post_data.decode('utf-8'))

        self.send_response(200)
        self.send_header('Content-type', 'text/plain')
        self.end_headers()
        self.wfile.write(b"POST request received!")

if __name__ == "__main__":
    server_address = ('', 8443)  # 监听 8443 端口
    httpd = HTTPServer(server_address, SimpleHTTPRequestHandler)

    # 创建 SSL 上下文
    context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    context.load_cert_chain(certfile='cert.pem', keyfile='key.pem')

    # 将套接字升级为 TLS
    httpd.socket = context.wrap_socket(httpd.socket, server_side=True)

    print("Starting HTTPS server on https://localhost:8443")
    httpd.serve_forever()
