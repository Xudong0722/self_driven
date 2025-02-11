package main

import (
    "fmt"
    "net/http"
)

func handler(w http.ResponseWriter, r *http.Request) {
    // 输出请求方法和路径
    fmt.Printf("Received %s request for %s\n", r.Method, r.URL.Path)
    
    // 读取请求头
    for name, values := range r.Header {
        fmt.Printf("Header: %s, Value: %s\n", name, values)
    }
    
    // 简单返回响应
    w.WriteHeader(http.StatusOK)
    w.Write([]byte("Hello, this is a response from the Go server!"))
}

func main() {
    http.HandleFunc("/", handler)
    
    fmt.Println("Starting server on :8080...")
    if err := http.ListenAndServe(":8080", nil); err != nil {
        fmt.Println("Error starting server:", err)
    }
}
