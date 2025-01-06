# Sistemas Operacionais: Simulador da Arquitetura de Von Neumann e Pipeline MIPS

<div style="display: inline_block">
  <img align="center" alt="VS" src="https://img.shields.io/badge/Visual_Studio_Code-0078D4?style=for-the-badge&logo=visual%20studio%20code&logoColor=white" />
  <img align="center" alt="Linux" src="https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black" />
  <img align="center" alt="C++" src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
</div><br/>

## Sobre o Projeto

Este projeto implementa um simulador da Arquitetura de Von Neumann com suporte a múltiplos núcleos, escalonamento e gerenciamento de recursos, utilizando um pipeline MIPS de 5 estágios (IF, ID, EX, MEM, WB). O simulador foi desenvolvido como parte do estudo de Sistemas Operacionais e visa explorar conceitos como execução paralela, escalonamento de processos e otimização de recursos.

---

## Estrutura do Projeto

O projeto utiliza arquivos de configuração e instruções para definir os cenários de simulação. Os principais arquivos são:

- **[`instructionsN.txt`](https://github.com/celzin/Simulador_SO/tree/main/data/instr)**: Contém as instruções a serem simuladas, onde cada arquivo representa um processo específico.
- **[`setRegisters.txt`](https://github.com/celzin/Simulador_SO/blob/main/data/setRegisters.txt)**: Define os valores iniciais dos registradores.
- **[`configBootloader.txt`](https://github.com/celzin/Simulador_SO/blob/main/data/configBootloader.txt)**: Configura os parâmetros de execução, como número de núcleos, diretório de logs e quantum dos processos.

### Exemplo de Arquivos de Configuração

#### instructionsN.txt

Cada arquivo `instructionsN.txt` especifica as instruções para um processo. O formato é:
`OPERAÇÃO, REGISTRADOR_DESTINO, REGISTRADOR_1, REGISTRADOR_2`.

Exemplo (`instructions1.txt`):

```
ADD, 1, 2, 3
SUB, 4, 1, 3
STORE, 3, 2, 0
LOAD, 0, 2, 0
```

Exemplo (`instructions2.txt`):

```
ENQ, 7, 3, 4
IF_igual, 8, 4, 7
```

#### setRegisters.txt

Define os valores iniciais dos registradores:

Exemplo:

```
2,10
3,5
4,7
```

#### configBootloader.txt

Configura o número de núcleos, limites de quantum e diretório de logs.

Exemplo:

```
NUM_NUCLEOS=4
QUANTUM_PROCESS_MIN=20
QUANTUM_PROCESS_MAX=50
OUTPUT_LOGS_DIR=data/logs
```

---

## Estrutura de Logs

Durante a execução, o simulador gera logs detalhados sobre o comportamento dos processos, uso de recursos e desempenho dos núcleos.

Os logs de execução serão gerados no diretório especificado em `configBootloader.txt` (por padrão: `data/logs/`).

- **`global_log.txt`**: Resumo geral da execução, incluindo tempos de execução e taxas de utilização dos núcleos.
- **`output_nucleo_<id>.txt`**: Logs individuais para cada núcleo, detalhando o progresso dos processos.

---

## Compilação e Execução

O projeto utiliza um `Makefile` para gerenciar a compilação e execução. A seguir, passo a passo para execução:

### Passos para Execução

1. Certifique-se de que os arquivos de configuração (`instructionsN.txt`, `setRegisters.txt`, `configBootloader.txt`) estejam corretamente configurados na pasta `data/`.
2. Para múltiplos processos, inclua todos os arquivos `instructionsN.txt` necessários no diretório `data/instr/`.
3. Navegue até o diretório raiz do projeto e execute os comandos abaixo:

---

### Comandos Disponíveis

| Comando      | Descrição                                                                                  |
| ------------ | ------------------------------------------------------------------------------------------ |
| `make clean` | Remove arquivos de compilação gerados anteriormente na pasta `build/`.                     |
| `make`       | Compila o código-fonte utilizando o GCC e armazena os binários na pasta `build/`.          |
| `make run`   | Executa o programa a partir dos binários gerados na pasta `build/`.                        |
| `make c`     | Realiza `make clean`, `make` e `make run` sequencialmente (recomendado para novos testes). |

## Contatos

| Participante   | Contato                                                                                                                                                                                |
| -------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Celso Vinícius | <a href="https://github.com/celzin"><img align="center" height="20px" width="90px" src="https://img.shields.io/badge/GitHub-181717?style=for-the-badge&logo=github&logoColor=white"/>  |
| Pedro Dias     | <a href="https://github.com/peudias"><img align="center" height="20px" width="90px" src="https://img.shields.io/badge/GitHub-181717?style=for-the-badge&logo=github&logoColor=white"/> |
