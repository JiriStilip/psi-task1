# 1. úloha z předmětu KIV/PSI - Implementace vícevláknového TCP serveru a odpovídajícího klienta

## Popis
Server přijímá řetězec o maximální délce 255 znaků a posílá jej zpět klientovi obrácený. 
Délka řetězce se v obou případech nachází v prvním bytu zprávy.

## Sestavení
Sestavení obou aplikací lze provést příkazem `make` volaným v kořenovém adresáři.

## Spuštění
- `server <port>`
- `client <IP address> <port> <message>`
