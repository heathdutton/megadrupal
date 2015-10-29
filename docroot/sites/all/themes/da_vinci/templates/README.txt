TEMPLATES:
==========
The templates directory contains the theme templates, diferentiated and divided by folders. They are the structure of all the content that we can see.

In this way, each block, node, page, region, panel (by default or not) depart from their corresponding templates: 'block.tpl.php' 'node.tpl.php' 'page.tpl.php', 'region.tpl.php' ...

If we need to create a specific template for a content, we only have to add "--" to the end, with the name of our content type. For example, if we want to create a template only for the nodes of type article, we will create "node--article.tpl.php". 

Drupal is able to recognize automatically the change, in this way we can modify this new template without alter the rest of existing nodes.


============
  ESPAÑOL
============

TEMPLATES:
==========
El directorio templates contiene, diferenciadas y divididas por carpetas, las plantillas del tema. Son la estructura de todo el contenido que podemos ver.

De esta manera cada bloque, nodo, página, región, panel (por defecto o no) partiran de sus correspondientes plantillas: 'block.tpl.php' 'node.tpl.php' 'page.tpl.php', 'region.tpl.php'...

Si necesitamos crear una plantilla específica para un contenido tan solo tenemos que añadir "--" al final, seguido del tipo de contenido deseado. Por ejemplo, si queremos crear una plantilla solo para los nodos de tipo article crearemos "node--article.tpl.php".

Drupal es capaz de reconocer automaticamente el cambio, de esta manera podemos modificar esta nueva plantilla sin alterar el resto de nodos existentes.
