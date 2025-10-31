# 🧱 mineserver-c
A lightweight Minecraft server written in **C**, inspired by old Notchian Beta servers.  
Goal: make a basic login + spawn system, no plugins, no bloat.

---

## ⚙️ Features
- ✅ Config loading (`server.properties`)
- ✅ TCP socket setup
- ⬜ Packet handling (Handshake, Login)
- ⬜ Player spawn + KeepAlive
- ⬜ Basic world data

---

## 🧰 Build & Run
```bash
make
./server
```

If no config exists, it auto-creates:
```
ip=localhost
port=25565
```

---

## 🗺️ Roadmap
- [ ] Handshake + Login flow  
- [ ] Player Position + Look  
- [ ] KeepAlive packets  
- [ ] Chat support  
- [ ] Basic chunk sending  

---

## 🧍 Author
**Albi Zhaku** — NJIT CS Student  
> building a minecraft server in C cause why not
