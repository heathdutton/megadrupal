/**
 * @file
 * Particle Emitter: color picker trigger (farbtastic).
 */

(function ($) {
  Drupal.behaviors.particle_emitter_color_picker = {

    attach: function(context, settings) {

      function cpCanvas (color) {
        $("#edit-particle-emitter-canvas-bgcolor").val(color);
        /*var red = parseInt(color.substring(1,3),16);
        var green = parseInt(color.substring(3,5),16);
        var blue = parseInt(color.substring(5,7),16);*/
      };

      function cpParticles (color) {
        $("#edit-particle-emitter-particles-color-hex").val(color);
        /*var red = parseInt(color.substring(1,3),16);
        var green = parseInt(color.substring(3,5),16);
        var blue = parseInt(color.substring(5,7),16);*/
      };

      var cp_canvas = $.farbtastic('#particle_emitter_color_picker_canvas');
      cp_canvas.linkTo(cpCanvas);
      cp_canvas.setColor(settings.particle_emitter.canvas_bgcolor);

      var cp_particles = $.farbtastic('#particle_emitter_color_picker_particles');
      cp_particles.linkTo(cpParticles);
      cp_particles.setColor(settings.particle_emitter.particles_color_hex);
    }
  }
})(jQuery);
