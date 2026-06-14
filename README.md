# mineserver-c

An experimental Minecraft Beta 1.7.3-compatible server written in C.
<img width="1932" height="1240" alt="image" src="https://github.com/user-attachments/assets/905e6dc3-cfb3-4476-9086-4cf1da33b085" />
> Screenshot is a placeholder from the actual game, not the server implementation.

## Design 
<img width="2202" height="1025" alt="Design" src="https://github.com/user-attachments/assets/28a0f01a-e04b-4c38-a165-38ec1fc73cb8" />
The server is structured as a packet processing pipeline. The input is read from a non-blocking socket, then assembled into packets which get parsed, validated and passed to the game logic layer, which updates the master game state 

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
  <li>
    <a href="https://fabiensanglard.net/quake3/network.php">
      https://fabiensanglard.net/quake3/network.php
    </a>
  </li>
  <li>
    <a href="https://gafferongames.com/">https://gafferongames.com/</a>
  </li>
  <li><a href="https://beej.us/guide/bgnet/pdf/bgnet_usl_c_1.pdf">https://beej.us/guide/bgnet/pdf/bgnet_usl_c_1.pdf</a></li>
</ul>


