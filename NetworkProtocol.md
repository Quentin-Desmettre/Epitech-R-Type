
# AECS - Network Protocol

This document describes the network protocol used by AECS to communicate between clients and the server.

The client and the server mainly communicate over a UDP connection, and use TCP for the initial handshake.

# Protocol

- [Step 1: Establishing a connection](#step-1-establishing-a-connection)
- [Step 2: Sending data, client side](#step-2-sending-data-client-side)
- [Step 3: Receiving data, server side](#step-3-receiving-data-server-side)
- [Step 4: Sending data, server side](#step-4-sending-data-server-side)
- [Step 5: Receiving data, client side](#step-5-receiving-data-client-side)
- [Packet Structures](#packet-structures)

## Step 1: Establishing a connection

Before the client runs the game, it must first get the game state from the server. To do so, it connects to the server using TCP.

Once the server has detected the connection, it will send a [`CONNECTED`](#connected-packet) packet to the client, containing the game state as encoded by `aecs::GameState::encode()`.

## Step 2: Sending data, client side

At the end of each game tick, the client must send a [`GAME_INPUT`](#gameinput-packet) packet to the server, which contains the client's input for that game tick.

However, if no input was received from the user for more than 1000ms, the client must send a [`PING`](#ping-packet) packet to the server, to notify it that the client is still connected.

## Step 3: Receiving data, server side

At the beginning of each game tick, the server will check if it received any data from any client. This data can be of 3 kinds:

- A [`GAME_INPUT`](#gameinput-packet) packet, which contains the client's input for that game tick.
- A [`CLIENT_PONG`](#clientpong-packet) packet, which is a response to the server's [`GAME_CHANGES`](#gamechanges-packet) packet.
- A [`PING`](#ping-packet) packet, which is a simple ICMP echo request.
  - If the server receives a [`PING`](#ping-packet) packet, it must respond with a [`SERVER_PONG`](#serverpong-packet) packet instantly.

If a [`GAME_INPUT`](#gameinput-packet) packet was received, the server must perform the actions described by the packet on the game tick it currently is.
That is to say, if the client was at tick 20 when it sent the packet, and the server is at tick 30, the server will perform the actions described by the packet on tick 30.

> No matter what type of packet was received, when the server receives a packet from a client, its disconnect timer is reset.

> **_WARNING_**  If the client did not send a [`GAME_INPUT`](#gameinput-packet), a [`CLIENT_PONG`](#clientpong-packet) or a [`PING`](#ping-packet) packet for 5 seconds, the server will disconnect the client.

## Step 4: Sending data, server side

At the end of each game tick, the server will send a [`GAME_CHANGES`](#gamechanges-packet) packet to every client, which contains the entity that changed during that game tick along with the game tick.

Also, if the server has not received a [`CLIENT_PONG`](#clientpong-packet) packet from a client for one of the last game changes it sent, it will send these changes again, included in the [`GAME_CHANGES`](#gamechanges-packet) packet.

## Step 5: Receiving data, client side

At the beginning of each game tick, the client will check if it received any data from the server, which can only be a [`GAME_CHANGES`](#gamechanges-packet) packet.

When the client receive this packet, it must send to the server a [`CLIENT_PONG`](#clientpong-packet) packet, which contains the game tick of the last change it received (as a [`GAME_CHANGES`](#gamechanges-packet) packet can contain information on multiple ticks).

The client may then push these changes to a queue, and dequeue them 30ms later, to compensate a 30ms ping.


# Packet Structures

Every packet follows the follow architecture:

```
+-------------+-------------+-------------+
| 2 bytes     | 1 byte      | n bytes     |
+-------------+-------------+-------------+
| packet size | packet type | packet data |
+-------------+-------------+-------------+
```

With the following definitions:

- `packet size`: The size of the packet, in bytes. This includes the packet type and the packet data, but not the packet size itself.
    > **_NOTE:_**  `packet size` is stored on 2 bytes, as the UDP protocol can only send packets of up to ~65,535 bytes, which number fits in 2 bytes.
- `packet type`: The type of the packet. This is a single byte, which can be one of the following:
  - Server packets:
    - `0x80`: `CONNECTED`
    - `0x81`: `GAME_CHANGES`
    - `0x82`: `SERVER_PONG`
  - Client packets:
    - `0x00`: `GAME_INPUT`
    - `0x01`: `PING`
    - `0x02`: `CLIENT_PONG`

**As so, the following section will only describe how `packet data` is structured.**

## `CONNECTED` packet

This packet contains the client ID, followed by the game state (same as [`GAME_STATE`](#gamestate-packet) packet).

```
+-----------+------------+
| 1 byte    | n bytes    |
+-----------+------------+
| client ID | game state |
+-----------+------------+
```

## `GAME_STATE` packet

A game state is encoded as follows:

```
+---------+--------------------+----------+
| 4 bytes | 4 bytes            | n bytes  |
+---------+--------------------+----------+
| tick    | number of entities | entities |
+---------+--------------------+----------+
```

Entities are encoded as follows:
```
+----------------+----------------------+------------+
| 4 bytes        | 2 bytes              | n bytes    |
+----------------+----------------------+------------+
| entity ID      | data size            | components |
+----------------+----------------------+------------+
```

If the entity has to be removed, `data size` will be `0`, and `components` will be empty.

Components are encoded as follows:
```
+--------------+-----------+---------+
| 2 bytes      | 2 bytes   | n bytes |
+--------------+-----------+---------+
| component ID | data size | data    |
+--------------+-----------+---------+
```

## `GAME_CHANGES` packet

This packet contains the number of changes, followed by the changes themselves. Each change is a [`GAME_STATE`](#gamestate-packet) packet, restricted to the entities that changed.

```
+----------------+-------------+-----------+-------------+-------------+-----+
| 1 byte         | n bytes     | n bytes   | n bytes     | n bytes     | ... |
+----------------+-------------+-----------+-------------+-------------+-----+
| change count   | change 1    | change 2  | change 3    | change 4    | ... |
+----------------+-------------+-----------+-------------+-------------+-----+
```

## `SERVER_PONG` packet

This packet does not contain any data, as the fact that it is the `SERVER_PONG` packet is enough to know that the server is still connected.

## `GAME_INPUT` packet

This packet contains, in order:

- The number of inputs, as an 8-bit unsigned integer.
- The inputs themselves, as 8-bit unsigned integers.

```
+-------------+------------+-----+
| 1 byte      | 1 byte     | ... |
+-------------+------------+-----+
| input count | input      | ... |
+-------------+------------+-----+
```

## `PING` packet

This packet does not contain any data, as the fact that it is the `PING` packet is enough to know that the client is still connected.

## `CLIENT_PONG` packet

This packet contains the game tick of the last change the client received.

```
+---------+
| 4 bytes |
+---------+
| tick    |
+---------+
```
