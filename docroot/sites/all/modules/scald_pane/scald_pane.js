(function ($) {
  Drupal.behaviors.sclad_pane = {
    attach: function (context, settings) {
      $("#scald-pane-edit-form #edit-scald-atom", context).each(function() {
        var sid = $(this).val();
        if (sid.length > 0) {
          var init_type = $("input[name='scald_atom_type']").val();
          if (typeof init_type !== "undefined" && init_type) {
            sclad_pane_set_representation(sid, init_type);
          }
          else {
            sclad_pane_set_representation(sid);
          }

        }
        else {
          sclad_pane_set_representation(0);
        }

        $('#scald-pane-edit-form #edit-scald-atom').change(function(e) {
          var sid = $(this).val();
          if (sid.length > 0) {
            sclad_pane_set_representation(sid);
          }
          else {
            sclad_pane_set_representation(0);
          }
        });
      });

      function sclad_pane_set_representation(sid, init_type) {
        if ( typeof Drupal.dnd.Atoms[sid] !== "undefined" && Drupal.dnd.Atoms[sid]) {
          $("#scald-pane-edit-form .form-item-scald-context").show();
          if (typeof init_type !== "undefined" && init_type) {
            var atom_type = init_type;
          }
          else {
            var atom_type = Drupal.dnd.Atoms[sid].meta.type;
          }

          if (typeof Drupal.settings.dnd.contexts[atom_type] !== "undefined" && Drupal.settings.dnd.contexts[atom_type]) {
            var scald_context = $('#scald-pane-edit-form .form-item-scald-context #edit-scald-context').val();

            $('#scald-pane-edit-form .form-item-scald-context #edit-scald-context').find('option').remove();

            $.each( Drupal.settings.dnd.contexts[atom_type], function( key, value ) {
              $('#scald-pane-edit-form .form-item-scald-context #edit-scald-context').append(
                new Option(value, key)
              );
            });

            $('#scald-pane-edit-form .form-item-scald-context #edit-scald-context').val(scald_context);
          }
        }
        else {
          $("#scald-pane-edit-form .form-item-scald-context").hide();
        }
      }
    }
  };
})(jQuery);
