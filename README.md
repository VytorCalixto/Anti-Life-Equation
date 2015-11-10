# CI164 - Trabalho Prático (1/2) - 2015-2
1º Trabalho Prático: Solução de uma Equação Diferencial Parcial Elíptica

---

O objetivo deste trabalho é implementar um programa computacional que calcule a 
solução discreta para a equação Diferencial Parcial definida [AQUI](http://www.inf.ufpr.br/danielw/grad/ci164/20152/ci164-Trabalho-01.pdf).

Para tal, o programa deve utilizar **Diferenças Finitas e o Método de Successive Over Relaxation (SOR) com fator de relaxação  ω = (2 - (hx+hy)) / 2**.

O **critério de parada** deve ser o **número máximo de iterações**;

A norma L² do resíduo (||r||) deve ser calculada a cada iteração, bem como o valor máximo;)

## Execução do Programa
O pacote de software a ser construído deve gerar um executável chamado **pdeSolver**, que deve ser invocado da seguinte forma:

```bash
pdeSolver -hx <Hx> -hy <Hy> -i <maxIter> -o arquivo_saida
```

Onde:
* **-hx**: parametro obrigatório definindo o tamanho do intervalo entre pontos na dimensão X (em radianos).
* **-hy**: parametro obrigatório definindo o tamanho do intervalo entre pontos na dimensão Y (em radianos).
* **-i maxIter**: parametro obrigatório definindo o número máximo de iterações a serem executadas.
* **-o arquivo_saida**: parametro obrigatório no qual **arquivo_saida** é o caminho
completo para o arquivo que vai conter a solução (valores da função em cada ponto da grade).
    * Esta solução deve estar formatada para servir de entrada ao comando gnuplot,
    de forma que ele possa automaticamente gerar o gráfico da função quando invocado da seguinte forma:

    ```bash
    $ gnuplot arquivo_saida
    ```

Além disso, no início do arquivo, deve constar sob a forma de comentários do gnuplot:

1. O tempo médio de execução de cada iteração do Método de SOR;
* O tempo médio para o cálculo da norma do resíduo;
* O valor do resíduo para cada iteração.

```bash
###########
# Tempo Método SOR: <média de tempo para o cálculo de uma iteração do método, em segundos>
# Tempo Resíduo: média de tempo para o cálculo do resíduo em segundos
#
# Norma do Resíduo
# i=1: <valor>
# i=2: <valor>
# i=3: <valor>
# ...
###########
```

* **Tempo**: deve ser calculado em segundos, utilizando-se a função especificada.
* **Tempo Método**: tempo calculado a partir do início da iteração do método até a obtenção do vetor solução daquela iteração. O resultado deve ser a média aritmética do tempo de todas iterações.
* **Tempo Resíduo**: Tempo para calcular a norma L² do resíduo. O resultado deve ser a média aritmética do tempo de todas iterações.

### Mensagens de Erro:
Em caso de erros, uma mensagem explicando o ocorrido deve ser impressa em **stderr** e a execução do programa deve ser encerrada com código **diferente** de **0**.

## Produto a ser Entregue
O trabalho deve ser desenvolvido por um grupo composto por no máximo DOIS alunos regularmente matriculados na disciplina. O grupo NÃO PODE SER ALTERADO na próxima parte do trabalho.

Cada grupo deve entregar um pacote de software completo contendo os fontes em linguagem C. O pacote deve ser arquivado e compactado com tar(1) e gzip(1) em um arquivo chamado login1.tar.gz (se grupo com 1 membro) ou login1-login2.tar.gz (se grupo com 2 membros), onde login1 e login2 são os logins dos alunos que compõem o grupo.

O pacote deve ter a seguinte estrutura de diretório e arquivos:
* ./login1-login2/: diretório principal
* ./login1-login2/LEIAME;
* ./login1-login2/Makefile;

Note que a extração dos arquivos de login1-login2.tar.gz deve criar o diretório login1-login2 contendo todos os arquivo acima. Os arquivos fonte também devem estar contidos no diretório, ou em algum sub-diretório, desde que o Makefile funcione.

**LEIAME**

O pacote deve conter um arquivo de documentação em texto plano (ASCII). Este arquivo, chamado LEIAME, deve conter as seguintes informações:

* autoria do software, isto é, nome e RA dos membros do grupo;
* lista dos arquivos e diretórios contidos no pacote e sua descrição (breve);
* um capítulo especia descrevendo os algoritmos e as estruturas de dados utilizadas
as alternativas de implementação consideradas e/ou experimentadas e os motivos
que o levaram a optar pela versão entregue, as dificuldade entregadas e as maneiras
pelas quais foram contornadas
* bugs conhecidos

**Makefile**

O arquivo Makefile deve possuir as regras necessárias para compilar os módulos
individualmente e gerar o programa executável. As seguintes regras devem existir OBRIGATORIAMENTE:

* **all**: compila e produz um executável chamado **pdeSolver** no diretório login1-login2/;
* **clean**: remove todos os arquivos temporários e os arquivos gerados pelo Makefile (*.o, executável, etc)

#### Entrega
O prazo final para a entrega deste trabalho é dia **15 de novembro de 2015, 23:59:59h**, IMPRETERIVELMENTE

* O trabalho deve ser enviado como anexo ao email do professor com o Assunto (Subject): **CI164 - Trabalho 1**;
* No corpo da mensagem DEVE CONSTAR OBRIGATORIAMENTE o Nome e Números de Registro Acadêmico (RA) dos membros do grupo;
* O grupo deverá considerar o trabalho como entregue SOMENTE APÓS RECEBER DO PROFESSOR UMA MENSAGEM DE CONFIRMAÇÃO
DE RECEBIMENTO dentro de 24 horas desde o envio da mensagem;

#### Critérios de Avaliação
APENAS OS TRABALHOS QUE FUNCIONAREM SERÃO CORRIGIDOS. Se o trabalho não compilar ou acusar falha de segmentação (Segmentation fault) prematura durante os testes realizados pelo professor (sem que qualquer operação se efetue a contento), trará para o grupo NOTA 0 (ZERO). Também receberão NOTA 0 (ZERO) trabalhos plagiados de qualquer fonte, e/ou com códigos similares. Além disso, apenas trabalho entregues no prazo marcado receberão nota.

Os itens de avaliação do trabalho e respectivas pontuações são:
* Qualidade da documentação: arquivo LEIAME (10 pontos)
* Entrada e saída: funcionamento do programa de acordo com a especificação no que tange execução, entrada e saída (20 pontos)
* Funcionamento: corretudo das respostas nos testes executados (50 pontos)
* Eficiência das estruturas de dados utilizadas, desde que devidamente justificadas no arquivo LEIAME (20 pontos)

**Defesa:** A defesa do trabalho será oral, e definirá a nota individual de cada membro da equipe, de acordo com seu conhecimento a respeito do trabalho.

#### Função para Medição de Tempo
O tempo de execução deve ser medido em segundos, considerando tempo de relógio,
utilizando a funcção especificada abaixo:
```C
double timestamp(void) {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double)(tp.tv_sec + tp.tv_usec/1000000.0));
}
```
onde o tempo decorrido é medido pela diferença do "timestamp" medidos antes e depois da região de interesse.
