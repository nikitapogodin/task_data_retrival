docker rm -f server
docker build -t server .
docker run -dp 8080:8080 server

