# ProjetoCompilador -> flex-yacc-bison
    Projeto acadêmico.

    > João Guilherme Saraiva Nogueira - 4° Período de Ciência da Computação.
    > Matheus Henrique Jonas De Oliveira - 4° Período de Ciência da Computação.
    
O objetivo deste trabalho era entender como os compiladores funcionam, de forma prática. Tivemos contato com todas as fazes que constituem o processo de compilação, desde a análise léxica até a otimização final.

Criamos, com a ajuda do Professor Cláudio Faria, docente da matéria de compiladores, este compilador, que traduz uma linguagem parecida com C para código MIPS. 
    
Exemplo de código aceito: 

```c
        int main()
        {
            int a, b, c;
            a = read();
            b = read();
            c = read();
            soma(a, b, c);
        }

        void soma(int d, int e, int f)
        {
            println(d + e + f);
        }
```
Saída: 

```
18
24
4
```

Rodando o projeto: (Windows)
```
    flex analex.l
    bison -o mycomp.c mycomp.y -d
    gcc mycomp.c -o mycomp.exe
```

Limitações: 
- Registradores limitados, logo, variáveis limitadas;
- Funções de no máximo 4 parâmetros;
- Identificadores de parâmetros de funções devem ser diferentes dos identificadores utilizados no código.

Exemplo:

```c
        int main()
        {
            int a;
            a = 4;
            soma10ePrinta(a);
            println(a);
        }

        void soma10ePrinta(int a)
        {
            a = a + 10;
            println(a);
        }

```
Saída: 

```
14
14
```

Os valores são alterados.
