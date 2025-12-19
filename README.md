# mineserver-c
A lightweight Minecraft server written in **C**, inspired by old Notchian Beta servers.  

<img width="966" height="620" alt="Screenshot 2025-12-19 at 3 22 51 AM" src="https://github.com/user-attachments/assets/a9d1d8c1-399a-4e5b-a530-a6b6c619a1dd" />

# Current Status
Still in early development! 
<br><br>
Right now the client can connect to the server and disconnect just fine. Currently working on packet parsing.

# Build & Run
In order to build this program you must run the makefile with 
```
make
``` 
Then you simply start the server by running 
```
./server.out
```
# Server Properties
The server properties while still not at its final state has some relevant settings for the server
```
ip=localhost
port=25565
max-players=10
```
Here you can change the IP, port number, and the total number of players allowed on the server. 
For now, I recommend keeping the max players between 10 and 50, since this server is currently single-threaded using pollfd (so it isn’t very fast!).

# Resources
Some websites I found useful!
<ul>
  <li>
    <a href="https://pixelbrush.dev/beta-wiki/">
      https://pixelbrush.dev/beta-wiki/
    </a>
  </li>
  <li>
    <a href="https://minecraft.wiki/w/Minecraft_Wiki:Projects/wiki.vg_merge/Protocol?oldid=2769758">
      https://minecraft.wiki/w/Minecraft_Wiki:Projects/wiki.vg_merge/Protocol?oldid=2769758
    </a>
  </li>
</ul>

