/**
 * @file
 * Interacting with the admin panel.
 */

var wp_tarteaucitron = {
  "deleteService": function (id_uniq) {
    "use strict";
    document.getElementById('delete_' + id_uniq).value = '1';
    document.getElementById('block-admin-configure').submit();
  },

  "selectService": function (id, id_uniq) {
    "use strict";
    wp_tarteaucitron.toggle('addit' + id);
    wp_tarteaucitron.toggle('hoveradd_b' + id);
    if (document.getElementById('addit' + id).style.display === 'none') {
      document.getElementById('check' + id).checked = false;

      if (id_uniq !== undefined) {
        document.getElementById('img_' + id_uniq).src = '//opt-out.ferank.eu/img/services/000.png';
        document.getElementById('input_img_' + id_uniq).value = '000';
      }
    }
    else {
      document.getElementById('check' + id).checked = true;
    }

  },

  "toggle": function (id) {
    "use strict";
    if (document.getElementById(id).style.display === 'none') {
      document.getElementById(id).style.display = 'block';
    }
    else if (document.getElementById(id).style.display === 'block') {
      document.getElementById(id).style.display = 'none';
    }
  },

  "cssByClass": function (matchClass, content) {
    "use strict";
    var elems = document.getElementsByTagName('*'),
      i,
      index = 0;

    for (i in elems) {
      if (elems[i] !== undefined) {
        for (index = 0; index < matchClass.length; index += 1) {
          if ((' ' + elems[i].className + ' ')
              .indexOf(' ' + matchClass[index] + ' ') > -1) {
            elems[i].style.border = content;
          }
        }
      }
    }
  }
};
