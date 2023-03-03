all:
	gcc -o client_appl Client/client.c Client/user_interaction.c Shared/Packet_Manipulation/read_packets.c Shared/Packet_Manipulation/write_packets.c Shared/Packet_Manipulation/packets.c Shared/utils.c Shared/file_manipulation.c Shared/Data_Flow/send_data.c Shared/Data_Flow/receive_data.c
	gcc -o server_appl Server/server.c Server/connection_establishment.c Server/data_channel.c Shared/Packet_Manipulation/read_packets.c Shared/Packet_Manipulation/write_packets.c Shared/Packet_Manipulation/packets.c Shared/utils.c Shared/file_manipulation.c Shared/Data_Flow/send_data.c Shared/Data_Flow/receive_data.c
