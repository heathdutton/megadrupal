/*
 * File
 * Masonry instructions
 */

Como aplicar la librería Masonry a tu vista.

1º Creamos la vista con la siguiente configuración:
  - View name: "nombre"
  - Format: HTML list

2º Para aplicarle los estilos masonry nos dirigimos al settings del tema da_vinci "/admin/appearance/settings/da_vinci" e introducimos el nombre de nuestra vista en el formulario de activación.

- Activate masonry view: "nombre"

¡IMPORTANTE!

Cuando creamos la vista , drupal le añade una clase con la siguiente estructura:  .view-"nombre". Debemos excluir "view-" a la hora de introducir el nombre en el formulario de activación.

Ejemplo:
  - Nombre de la vista: "drupal"
  - Clase generada: ".view-drupal"
  - Activate masonry view: "drupal"

//////////////////////////////////////////////

How to apply the masonry library to your view.

1º Create a view with the following configuration:
  - View name: "name"
  - Format; HTML list

2º Go to da_vinci theme settings and insert the name of the view in the activation form:
  - Activate masonry view: "name"

IMPORTANT!
When we create the view, drupal adds a class in the view with the following structure: .view-"name". We have to exclude ".view-" in the activation formulary.

Example:
  - View name: "drupal"
  - Class name generated: ".view-drupal"
  - Activate masonry view: "drupal"