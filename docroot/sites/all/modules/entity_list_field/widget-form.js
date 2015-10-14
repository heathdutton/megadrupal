/**
 * @file
 * Widget form javascript.
 */

(function($) {
  Drupal.behaviors.entity_list_field = {
    attach: function(context, settings) {

      var entity_list_field = {

        viewModeOps: function(map, type, view_mode) {
          var vm_names = map[type]['view modes'];
          var options = '';
          for (k in vm_names) {
            if (view_mode == k) {
              s = ' selected="selected"';
            }
            else {
              s = '';
            }
            options = options + '<option value="' + k + '"' + s + '>' + vm_names[k].label + '</option>';
          }
          if (0 == options.length) {
            $('select#entity_list_field-view_mode-' + delta).parent().hide();
          }
          else {
            $('select#entity_list_field-view_mode-' + delta).parent().show();
            $('select#entity_list_field-view_mode-' + delta).html(options);
          }
        },

        sortOps: function(map, type, sort, sort2) {

          var ops = {};
          var s;
          var props = map[type]['properties'];

          for (p in props) {
            if ('undefined' !== typeof(props[p]['schema field'])) {
              s = props[p]['schema field'];
              ops[s] = Drupal.t('property') + ': ' + props[p].label;
            }
          }

          var bnd;
          var bundles = entity_list_field.bundleVals();
          var fops = {};

          for (b in bundles) {
            bnd = bundles[b];
            var fields = map[type]['bundles'][bnd];
            for (f in fields) {
              fops[f] = Drupal.t('field') + ': ' + f;
            }
          }

          for (b in bundles) {
            bnd = bundles[b];
            for (f in fops) {
              // For each field in ops, make sure it's in this bundle.
              if ('undefined' == typeof map[type]['bundles'][bnd][f]) {
                delete fops[f];
              }
            }
          }

          ops = entity_list_field.collect(ops, fops);

          var options = '<option value="entity_list_field_na">-' + Drupal.t('Select') + '-</option>';
          var options2 = options;

          for (n in ops) {

            if (sort == n) {
              s = ' selected="selected"';
            }
            else {
              s = '';
            }
            options = options + '<option value="' + n + '"' + s + '>' + ops[n] + '</option>';

            if (sort2 == n) {
              s2 = ' selected="selected"';
            }
            else {
              s2 = '';
            }

            options2 = options2 + '<option value="' + n + '"' + s2 + '>' + ops[n] + '</option>';

          }

          $('select#entity_list_field-sort-' + delta).html(options);
          $('select#entity_list_field-sort2-' + delta).html(options2);

        },

        collect: function() {
          var ret = {};
          var len = arguments.length;
          for (var i=0; i<len; i++) {
            for (p in arguments[i]) {
              if (arguments[i].hasOwnProperty(p)) {
                ret[p] = arguments[i][p];
              }
            }
          }
          return ret;
        },

        bundleOps: function(map, type, uncheck) {

          if ('undefined' !== typeof map[type]['bundles']) {
            var bundles = map[type]['bundles'];
            $('div#bundle_select-input-wrapper-' + delta).show();
          }
          else {
            var bundles = [];
            $('div#bundle_select-input-wrapper-' + delta).hide();
          }

          $('div#bundle_select-input-wrapper-' + delta + ' input').each(function(index) {
            var cb_val = $(this).val();
            if ('undefined' == typeof bundles[cb_val]) {
              $(this).parent().hide();
            } else {
              $(this).parent().show();
            }
            if (uncheck) {
              $(this).attr({checked: false});
              if (0 === index) {
                $('#entity_list_field-bundle-' + delta).val('');
              }
            }
          });
        },

        bundleVals: function() {
          bundle_vals = new Array();
          $('#bundle_select-input-wrapper-' + delta + ' input').each(function() {
            if ($(this).attr('checked')) {
              bundle_vals.push($(this).val());
            }
          });
          return bundle_vals;
        },

        setBundleVal: function() {
          bundle_vals = entity_list_field.bundleVals();
          $('#entity_list_field-bundle-' + delta).val(JSON.stringify(bundle_vals));
        },

        columns: function(map, type, delta, num) {
          if ('undefined' === typeof num) {
            num = '';
          }
          var s = $('select#entity_list_field-sort' + num + '-' + delta).val();

          var bundle_vals = entity_list_field.bundleVals();

          var fields = [];
          for (var b = 0; b < bundle_vals.length; b++) {
            bundle = bundle_vals[b];
            for (var f in map[type]['bundles'][bundle]) {
              fields.push(f);
            }
          }

          if (-1 !== $.inArray(s, fields)) {
            $('#entity_list_field-column' + num + '-' + delta).parent().show();
          }
          else {
            $('#entity_list_field-column' + num + '-' + delta).parent().hide();
          }
        }
      };

      var delta = settings.entity_list_field.delta;
      var map = settings.entity_list_field.map;
      var type = $('select#entity_list_field-type-' + delta).val();
      var view_mode = settings.entity_list_field.view_mode;
      var sort = settings.entity_list_field.sort;
      var sort2 = settings.entity_list_field.sort2;

      entity_list_field.bundleOps(map, type, false);
      entity_list_field.viewModeOps(map, type, view_mode);
      entity_list_field.sortOps(map, type, sort, sort2);
      entity_list_field.setBundleVal();

      // When the type changes, hide the irrelevant bundles and view modes.
      $('select#entity_list_field-type-' + delta).change(function() {
        type = $(this).val();
        entity_list_field.bundleOps(map, type, true);
        entity_list_field.viewModeOps(map, type, view_mode);
        entity_list_field.sortOps(map, type, sort, sort2);
      });

      // When a bundle value changes, hide the irrelevant sort options.
      $('#bundle_select-input-wrapper-' + delta + ' input').change(function() {
        entity_list_field.setBundleVal();
        type = $('select#entity_list_field-type-' + delta).val();
        entity_list_field.sortOps(map, type, sort, sort2);
      });

      entity_list_field.columns(map, type, delta);
      entity_list_field.columns(map, type, delta, 2);

      $('select#entity_list_field-sort-' + delta).change(function() {
        entity_list_field.columns(map, type, delta);
      });
      $('select#entity_list_field-sort2-' + delta).change(function() {
        entity_list_field.columns(map, type, delta, 2);
      });

    }
  };

})(jQuery);
