## Nome e ideia

dateUF: programa de relacionamento para estudantes. O usuário cadastra um
perfil e curte perfis de outras pessoas, que aparecem um por vez.

## O que o sistema guarda

O sistema guarda o perfil de cada pessoa: nome, curso, idade, gênero,
semestre, preferências e contato (WhatsApp e Telegram).

- idade: número inteiro, de 18 a 100
- gênero: lista fixa (Feminino, Masculino, Outro)
- curso: lista fixa definida no código
- semestre: número inteiro, de 1 a 12
- preferências: lista fixa, pode marcar várias (atividade física, beber,
  videogames, animes, esportes, leitura)
- contato: WhatsApp ou Telegram, texto livre, podem ficar em branco

Cada perfil recebe um número de identificação na ordem de cadastro. Nomes
podem se repetir; o número é que identifica.

## O que dá para fazer

- Cadastrar um perfil.
- Entrar como um perfil, informando o número dele.
- Ver os outros perfis um por vez e escolher "curtir" ou "pular".
- Aplicar filtros de curso, idade, gênero, semestre e preferências.
- Ver a lista de matches.
- Sair.

As curtidas são secretas e não podem ser desfeitas. Quando duas pessoas se
curtem, o sistema registra o match e revela o contato de uma para a outra.

## Regras

- Menor de 18 anos não pode se cadastrar.
- O perfil precisa ter nome.
- A pessoa não aparece na própria fila de recomendações.
- Não existe chat no programa: o contato só é revelado depois da curtida
  mútua.
- O filtro pode ser mudado a qualquer momento; ao mudar, a fila é montada
  de novo com os perfis que se encaixam.
- Se nenhum perfil se encaixar no filtro, ou se a fila acabar, o console
  mostra "sem mais recomendações por enquanto".
- Dados começam vazios a cada execução (não há arquivo).

## Quando algo dá errado

- "Entrar como" com número que não existe: avisa e volta ao menu.
- Cadastro com idade menor que 18: recusa e avisa.
- Nome em branco: recusa e avisa.
- Idade ou semestre que não seja número, ou fora da faixa: recusa e avisa.
- Opção de menu que não existe: avisa e mostra o menu de novo.
- Filtro sem nenhum resultado: mostra "sem mais recomendações por enquanto".
- Lista de matches vazia: mostra "você ainda não tem matches".
- Curtir alguém que já foi curtido: não faz nada e segue para o próximo.
- Texto digitado onde o programa espera número: avisa e volta ao menu, sem
  travar.

## Classes que eu imagino

- `Perfil`: guarda os dados de uma pessoa; sabe se é maior de 18 e compara
  perfis por nome.
- `Filtro`: guarda os critérios escolhidos e diz se um perfil se encaixa.
- `Curtidas`: guarda quem curtiu quem e diz quando houve match.
- `DateUF`: guarda os perfis, monta a fila de recomendações, registra
  curtidas e matches.

## Exemplo de uso

1. O programa começa vazio e mostra o menu.
2. O usuário cadastra três perfis e entra como o perfil 1.
3. O programa mostra o perfil 2; o usuário curte.
4. Mostra o perfil 3; o usuário pula.
5. Aparece "sem mais recomendações por enquanto".
6. O usuário sai, entra como perfil 2 e curte o perfil 1.
7. A lista de matches mostra o perfil 1 com o contato dele.