# Plano do dateUF

Plano de implementação do sistema descrito em `DESCRICAO.md`. Cada etapa
termina com algo que compila e que dá para testar. Nenhuma etapa começa
antes da anterior estar funcionando e commitada.

## 1. Resumo

Programa de console, C++11, só biblioteca padrão. O usuário cadastra perfis,
entra como um deles, vê os outros um por vez, curte ou pula, aplica filtros e
vê a lista de matches. Quando há curtida mútua, o contato dos dois é revelado.
Os dados ficam só na memória: cada execução começa vazia.

## 2. Classes e responsabilidades

| Classe | Guarda | Responsabilidade |
| --- | --- | --- |
| `Perfil` | id, nome, curso, idade, gênero, semestre, preferências, contato | representar uma pessoa; dizer se é maior de idade; comparar perfis |
| `Filtro` | curso, faixa de idade, gênero, faixa de semestre, preferências | dizer se um perfil se encaixa nos critérios |
| `Curtidas` | quem curtiu quem | registrar curtidas e detectar match |
| `DateUF` | perfis, curtidas, fila de recomendações | juntar tudo: cadastrar, montar fila, curtir, listar matches |
| `Interface` | usuário atual, filtro atual | ler do teclado e escrever na tela |

`Interface` não é classe do sistema: é a tela. Ela é a única que usa `cin` e
`cout`. As outras quatro recebem dados por parâmetro e devolvem resultado.

## 3. Containers da STL e por quê

| Onde | Container | Por quê |
| --- | --- | --- |
| `DateUF` | `map<int, Perfil>` | buscar perfil pelo número de identificação |
| `Curtidas` | `map<int, set<int> >` | para cada pessoa, o conjunto de quem ela curtiu |
| `Curtidas` | `set<int>` | não deixa a mesma curtida entrar duas vezes |
| `Perfil` | `set<string>` | preferências sem repetição e em ordem |
| `DateUF` | `list<Perfil>` | fila de recomendações: tira do começo e avança |
| `DateUF` | `vector<Perfil>` | lista ordenada para mostrar perfis e matches |
| `Perfil` | `operator<` | ordenar por nome (usado por `sort` e `list::sort`) |
| `Perfil` | `operator==` | comparar perfis pelo id (usado por `find`) |

Algoritmos usados:

- `sort` em `vector<Perfil>` para listar perfis e matches em ordem de nome;
- `list::sort()` na fila de recomendações (usa o mesmo `operator<`);
- `find` para procurar um perfil pelo id dentro de um `vector<Perfil>`.

Não vou usar `lambda`, `unordered_map`, `auto`, `new` nem `delete`.

## 4. Regras da descrição e onde cada uma fica

| Regra | Onde é garantida |
| --- | --- |
| menor de 18 não cadastra | `Perfil::maiorDeIdade()` + checagem na `Interface` |
| perfil precisa de nome | `Interface` recusa nome vazio |
| não aparece na própria fila | `DateUF::montarFila` ignora o id do usuário |
| curtida é secreta e não se desfaz | `Curtidas` só registra; não existe remover |
| match revela contato | `Curtidas::houveMatch` + `Interface` mostra os contatos |
| filtro pode mudar no meio | `Interface` chama `montarFila` de novo |
| fila vazia | `DateUF::filaVazia` + mensagem na `Interface` |
| dados começam vazios | nada é lido de arquivo |

## 5. Arquivos

```text
include/Perfil.hpp      src/Perfil.cpp
include/Filtro.hpp      src/Filtro.cpp
include/Curtidas.hpp    src/Curtidas.cpp
include/DateUF.hpp      src/DateUF.cpp
include/Interface.hpp   src/Interface.cpp
                        src/main.cpp
exemplos/*.txt
```

O comando do `README.md` já pega tudo:

```bash
g++ -std=c++11 -Wall -Wextra src/*.cpp -Iinclude -o sistema
```

No Windows (PowerShell), para rodar um cenário:

```powershell
Get-Content exemplos\cenario1.txt | .\sistema.exe
```

## 6. Etapas

### Etapa 1 — Classe `Perfil` e esqueleto do menu

**Arquivos:** `include/Perfil.hpp`, `src/Perfil.cpp`,
`include/Interface.hpp`, `src/Interface.cpp`, `src/main.cpp`,
`exemplos/cenario1.txt`.

**O que faz:**

- `Perfil` com os campos da descrição, getters e setters;
- `bool maiorDeIdade() const`;
- `bool operator<(const Perfil&) const` — compara pelo nome e, se empatar,
  pelo id;
- `bool operator==(const Perfil&) const` — compara pelo id;
- `bool temPreferencia(const string&) const`;
- `Interface::executar()` só mostra o menu e sai na opção `0`;
- `main` cria a `Interface` e chama `executar()`.

**Como testar:** compilar e rodar `cenario1.txt` (só o `0`). Deve aparecer o
menu e o programa encerrar sem erro.

**Commit:** `cria classe Perfil e esqueleto do menu`.

### Etapa 2 — Cadastro, "entrar como" e listagem

**Arquivos:** `include/DateUF.hpp`, `src/DateUF.cpp`; altera
`Interface.hpp` e `Interface.cpp`; novo `exemplos/cenario2_cadastro.txt`.

**O que faz:**

- `DateUF` com `map<int, Perfil> perfis` e `int proximoId`;
- `int cadastrarPerfil(const Perfil&)` — copia o perfil, dá o próximo id e
  devolve o id;
- `bool existePerfil(int) const`;
- `bool obterPerfil(int, Perfil&) const`;
- `vector<Perfil> listarPerfis() const` — copia do `map`, ordena com `sort`;
- menu: cadastrar, entrar como, listar perfis.

**Erros tratados nesta etapa:** nome vazio, idade menor que 18, idade fora de
18 a 100, semestre fora de 1 a 12, texto onde se espera número, "entrar como"
com id inexistente. Quando um campo é recusado, a tela mostra "Opcao invalida."
e pede o mesmo campo de novo, sem cancelar o cadastro. O contato é um campo
só: número de WhatsApp ou de Telegram.

**Como testar:** cadastrar dois perfis válidos, tentar um com 17 anos, tentar
um com idade `"abc"`, entrar com id inexistente, listar.

**Commit:** `adiciona cadastro de perfis e entrada por id`.

### Etapa 3 — `Filtro` e fila de recomendações

**Arquivos:** `include/Filtro.hpp`, `src/Filtro.cpp`; altera `DateUF` e
`Interface`; novo `exemplos/cenario3_filtro.txt`.

**O que faz:**

- `Filtro` com os cinco critérios; campo vazio ou zero significa "qualquer";
- `bool atende(const Perfil&) const` — idade e semestre por faixa; gênero e
  curso por igualdade; preferências por "pelo menos uma em comum";
- `DateUF::montarFila(int idUsuario, const Filtro&)` — limpa a `list`, copia
  os perfis que não são o usuário e que passam no filtro, e chama
  `fila.sort()`;
- `filaVazia()`, `proximoDaFila(Perfil&)`, `avancarFila()`;
- menu: ver próximo perfil, pular, mudar filtro (o "curtir" entra na etapa 4).

**Como testar:** três perfis, filtro por curso, filtro por idade, filtro por
preferências, filtro que não deixa ninguém (deve aparecer "sem mais
recomendações por enquanto"), trocar o filtro no meio e ver a fila ser refeita.

**Commit:** `adiciona filtros e fila de recomendacoes`.

### Etapa 4 — Curtidas, match e contato

**Arquivos:** `include/Curtidas.hpp`, `src/Curtidas.cpp`; altera `DateUF` e
`Interface`; novo `exemplos/cenario4_match.txt`.

**O que faz:**

- `Curtidas` com `map<int, set<int> >`;
- `void curtir(int de, int para)`;
- `bool jaCurtiu(int de, int para) const`;
- `bool houveMatch(int a, int b) const` — `a` curtiu `b` e `b` curtiu `a`;
- `DateUF::curtir`, `DateUF::deuMatch`;
- `montarFila` passa a ignorar os perfis que o usuário já curtiu;
- a tela de recomendações ganha a opção "Curtir";
- `vector<Perfil> listarMatches(int idUsuario) const` — filtra e ordena com
  `sort`;
- menu: ao curtir, avisar se deu match e mostrar os contatos dos dois; opção
  "ver matches".

**Como testar:** perfil 1 curte 2; entra como 2 e curte 1; o match aparece com
o contato. Curtir de novo não duplica. Ver matches com lista vazia mostra
"você ainda não tem matches".

**Commit:** `adiciona curtidas e matches`.

### Etapa 5 — Revisão e testes finais

**Arquivos:** ajustes pequenos; `README.md` (só a parte que o aluno escreve).

**O que faz:**

- rodar todos os arquivos de `exemplos/` em sequência;
- revisar mensagens de erro e listas vazias;
- compilar com `-Wall -Wextra` e corrigir os avisos;
- conferir que nenhuma classe do sistema usa `cin`/`cout`;
- preencher "Onde uso a STL" e "Como usei a IA" (aluno, sem IA).

**Commit:** `revisa mensagens e testes finais`.

## 7. Cenários de teste

| Arquivo | O que testa |
| --- | --- |
| `cenario1.txt` | menu abre e fecha |
| `cenario2_cadastro.txt` | cadastro válido, menor de 18, número inválido |
| `cenario3_filtro.txt` | filtros, fila vazia, troca de filtro |
| `cenario4_match.txt` | curtida, match, contato, lista vazia |

Depois de cada etapa, rodo **todos** os cenários, não só o novo.

## 8. O que fica de fora

- chat dentro do programa (a descrição diz que não existe);
- descurtir, ver quem curtiu, bloquear;
- senha ou login de verdade;
- salvar em arquivo (é o desafio 1, só depois do sistema pronto);
- interface gráfica.

## 9. Decisões que você pode mudar antes da etapa 1

- Ordem da fila: hoje é por nome. Se quiser, depois dá para ordenar por
  quantidade de preferências em comum.
- "Pular" só avança a fila atual. Se a fila for refeita (mudar o filtro ou
  entrar de novo em recomendações), o perfil pulado pode voltar. Só quem foi
  curtido fica fora da fila.
- Lista de cursos: fica no código, dentro da `Interface`.
- Faixas: idade 18 a 100, semestre 1 a 12 (como está na descrição).
