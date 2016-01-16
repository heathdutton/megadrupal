	Sass :
========
	|
	| Partials:
	===========
   Contains the stylesheets with 'sass' extension. Any change in the styles of the website and the creation of new stylesheets, can be modified in this directoy.

	 		|
	 		| Base:
	 		=======
      Base configuration for the theme: common styles and Susy settings.

	 		|
	 		| Components:
	 		=============
      Contains the styles for the different elements of the theme: buttons, error messages, modals, breadcrumbs...
	 		  |
	 			| Navigation:
	 			=============
        Styles for the main menu.

	 		|
	 		| Content:
	 		==========
      Here, we define the specific stylesheets for our content: nodes, views, browser styles, blocks, pages...

	 		|
	 		|	Lib:
	 		======
      Chosen's styles.

	 		|
	 		| Regions:
	 		==========
      Contains the styles for each region. If we create a new region here, its styles will be defined. Otherwise if we are going to give styles to a content inside a region, we should use the 'Content' directory.

	 		|
	 		| Utilities: 
	 		============
      This directory contains the variables, extends, mixins... that can be useful when personalizing our styles.

	====================================================================================================

	|
	| Main.sass:
	============
  All the previous styles will compile in a single file called 'main.sass'. This file will be converted into the final file: 'main.css'.
  We won't modify any style in this file, we are only able to add refferences to dependencies and also each file created in 'partials'.

	|
	| Chosen.sass:
	==============
	DON'T TOUCH !!!
	Compile all the Chosen's styles.



============
  ESPAÑOL
============

	Sass :
========
	|
	| Partials:
	===========
	 Contiene las hojas de estilos con extension 'sass', cualquier cambio en los estilos del portal, así como la creación de nuevas hojas de estilos, se hará en este directorio.

	 		|
	 		| Base:
	 		=======
	 		Configuración base del tema: estilos comunes y configuración de Susy.

	 		|
	 		| Components:
	 		=============
	 		Contiene los estilos para los diferentes elementos del tema: botones, mensajes de error, ventanas modales, migas de pan...
	 		  |
	 			| Navigation:
	 			=============
	 			Estilos para el menú principal.

	 		|
	 		| Content:
	 		==========
	 		Aquí se definen las hojas de estilos específicas para nuestro contenido: nodos, vistas, estilos para los diferentes navegadores, bloques, páginas...

	 		|
	 		|	Lib:
	 		======
	 		Estilos del chosen.

	 		|
	 		| Regions:
	 		==========
	 		Contiene los estilos para cada región. Si creamos una nueva región, aquí se definirán sus estilos; si por el contrario, vamos a darle estilos a un contenido dentro de una región, usaremos el directorio 'Content'.

	 		|
	 		| Utilities: 
	 		============
	 		Este directorio contiene las variables, extends, mixins... que nos serán de utilidad a la hora de personalizar nuestros estilos.

	====================================================================================================

	|
	| Main.sass:
	============
	Todos los estilos anteriores se compilan en un único archivo llamado 'main.sass'. Este archivo será transformado posteriormente al archivo final: 'main.css'.
  No modificaremos ningún estilo en este archivo, solo haremos referencia a aquellas dependencias necesarias, así como a cada archivo creado en 'partials'.

	|
	| Chosen.sass:
	==============
	¡¡¡ NO MODIFICAR !!!
	Compila los estilos del chosen.
