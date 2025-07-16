# KVStoreApp – A Distributed Key-Value Store

## 🚀 Overview

**KVStoreApp** is a lightweight, TCP-based distributed key-value store written in C++ using Boost.Asio.  
It supports concurrent client connections and synchronizes state across nodes in a cluster using a simple configuration file.

---

## ⚙️ Features

- ✅ **Multithreaded TCP Server**  
  Handles multiple client connections concurrently using a thread pool.

- 🧠 **In-Memory KV Store**  
  Fast `SET`, `GET`, and `DELETE` operations on a thread-safe in-memory store.

- 💾 **Persistent Storage**  
  Periodically (every 2 seconds) saves the current key-value data into a local SQLite `.db` file (`backup.db`).

- ♻️ **State Recovery on Restart**  
  Automatically loads key-value pairs from the database at startup to restore previous state.

- 🌐 **Clustered Node Communication**  
  On each `SET` or `DELETE`, the command is broadcasted to other nodes in the cluster using the peer configuration in `node_config.json`.

- 📌 **Configurable Startup via ID**  
  Start a node using:
  ```bash
  ./KVStoreApp --id=<node_id>
  ```
  The `node_id` must correspond to an entry in the `node_config.json` file, which contains each node’s IP and port.

---

## 🧪 Command Format

Commands are newline (`\n`) terminated strings sent via TCP.  
Supported commands:
- `SET <key> <value>`
- `GET <key>`
- `DELETE <key>`

Responses:
- `OK` (for successful `SET` or `DELETE`)
- The `<value>` for successful `GET`
- `KEY NOT FOUND` if a key does not exist
- `INVALID COMMAND` for invalid commands

---

## 🛠️ How to Build and Run

### 1. Clone and Build

```bash
git clone https://github.com/yourusername/KVStoreApp.git
cd KVStoreApp
cmake -S . -B build
cmake --build build
```

### 2. Edit Node Config

Modify `node_config.json` to include IPs and ports of all cluster nodes.  
Each node must have a unique ID based on its position in the array.

Example `node_config.json`:

```json
[
  { "ip": "192.168.1.5", "port": 5000 },
  { "ip": "192.168.1.6", "port": 5001 },
  { "ip": "192.168.1.7", "port": 5002 }
]
```

### 3. Run the Server

```bash
./build/app/KVStoreApp --id=0
```

This starts the server for the node with ID `0` as defined in `node_config.json`.

---

## 🔄 Node Communication

- On `SET` and `DELETE`, a broadcast message is sent to all other nodes in the cluster.
- Each peer maintains an async connection with retry logic.
- NodeManager is responsible for sending and receiving inter-node messages.

---

## ⏭️ Future Scope

- ⚡ Convert the TCP server to fully async Boost.Asio implementation.
- 🔐 Add authentication and encrypted communication (e.g., TLS).
- 📦 Add structured command protocol (e.g., RESP, JSON).
- 🧠 Implement consensus (Raft/Paxos) for stronger consistency.
- 🌐 Support public IPs with NAT traversal for global deployments.
- 📊 Add metrics, logging, and web-based dashboard for monitoring.
- 🧪 Add unit and integration tests for all core components.
- 📁 Add Dockerfile and Kubernetes deployment templates.

---

## 📄 License

MIT License – feel free to use, share, and improve.

---

## 🙋‍♂️ Author

Built with ❤️ by Soham Kanji
