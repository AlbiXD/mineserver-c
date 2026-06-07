# mineserver-c Bugs

## Fixed

### 2026-06-07 - Packet Assembly Without New Data

Problem:
- Packet assembly could run even if no bytes were read. (Example n=-1 on first read)

Fix:
- Added a `new_data` flag in which would be set to 1 when n>0.
- Only assemble packets when new data was received. (Since n>0 there is data that was read)