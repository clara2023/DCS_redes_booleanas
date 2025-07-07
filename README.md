# 



## Compilação BooleanNetwork
Na raíz do projeto
```console
make -C projects/GenesysTerminalApplication
```
## Executando
Na raíz do projeto
```console
 ./projects/GenesysTerminalApplication/dist/genesysterminalapplication 
```

Executar com parâmetros
```console
./projects/GenesysTerminalApplication/dist/genesysterminalapplication --bn='nodes=3;functions=[x1&x2,!x0,x0^x1]'
```
