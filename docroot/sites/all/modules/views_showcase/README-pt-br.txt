Descrição
===========
Esse módulo foi criado para suprir a necessidade de uma vitrine bonita, funcional e fácil de ser usada no Drupal.
Esse módulo é, de certa forma, um substituto para o Front Page Slideshow. A maior vantagem desse módulo sobre o FPS é que ele é feito com bibliotecas gratuitas.

Dependências
============
Esse módulo depende do Views para funcionar. Ele não terá efeito sozinho.

Instalação
============
Para instalar esse módulo siga os passos básicos:

* Baixe o módulo
* Descompacte em sites/all/modules ou no local onde desejar
* Acesse a área de administração de módulos do seu site (admin/build/modules)
* Habilite o módulo

Configuração
============
Para ter uma vitrine, é necessário criar uma View que irá gerar essa vitrine. Esse módulo pede ao menos 3 campos nessa view:

* Imagem - A imagem que será exibida no fundo
* Título - Título do node a ser exibido na página
* Teaser - Texto curto a ser exibido a cada node

Assim, crie a view normalmente com pelo menos esses 3 campos e escolha em Style o tipo Views Showcase e em seguida configure os parâmetros que desejar. Em seguida vá em Row style e escolha também Views Showcase. Nesse momento você deverá escolher qual campo irá usar em cada slot.

Você pode criar uma view que tenha tanto um display de bloco quanto um display de página, sendo que o display de bloco é mais versátil, pois pode ser posicionado em vários locais.

Autor
=====

Rafael Ferreira silva

http://rafaelsilva.net
http://drupal-br.org

Sou o criador e mantenedor do site brasileiro da comunidade de usuários de Drupal.
