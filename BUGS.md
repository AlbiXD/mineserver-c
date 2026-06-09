# mineserver-c Bugs

## Fixed


### 2026-06-08 - Packet Assembly Without New Data

Problem:
- bytes_read was not getting updated.
Fix:
- Moved cl->bytes_read = bytes_read when it detects a read, before I had accidentally left it in the drain while loop at the bottom so it was never getting reached.

### 2026-06-07 - Packet Assembly Without New Data

Problem:
- Packet assembly could run even if no bytes were read. (Example n=-1 on first read)

Fix:
- Added a `new_data` flag in which would be set to 1 when n>0.
- Only assemble packets when new data was received. (Since n>0 there is data that was read)