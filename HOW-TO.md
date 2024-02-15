
---

## Server Building and Running Instructions

### 1. Build the Server

#### a. Clone the Git Repo

```bash
git clone git@github.com:nikitapogodin/task_data_retrival.git
```

#### b. Create the Build Directory

```bash
mkdir -p task_data_retrival/build/release && cd task_data_retrival/build/release
```

#### c. Make

```bash
cmake -DCMAKE_BUILD_TYPE=Release ../.. && make -j 8
```

### 2. Run the Server

#### a. From Command Line

```bash
server --port <port to listen> --data_sources_number <number of datasources> --data_lifetime <data lifetime in ms>
```

Default values:
- `port` = 8080
- `data_sources_number` = 2
- `data_lifetime` = 250 ms

#### b. Build and Run Docker Container

```bash
./build_run_server_docker.sh
```

This will create a Docker image named 'server' and start the container.


### 3. Run the Client

```bash
python3 client/client.py
```

The client will connect to the server and request the data from source 1 repeatedly every 50ms.

### 3. Protocol

#### a. Request Format

Text data with the desired number of data source to get the data.

#### b. Response Format

Text data in JSON format:

```json
{
    "status": "OK",
    "data": 78
}
```

Attributes:

- `status`: Message describes the result of data retrieval.
- `data`: Value of retrieved data or null in case of any error occurred.

---
