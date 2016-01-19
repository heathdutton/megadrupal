/**
 * @file
 * Particle Emitter.
 */

Drupal.behaviors.particle_emitter = {

  attach: function (context, settings) {

    // console.debug(settings);
    // console.debug(context);

    // Globals.
    var particles = [];
    var mouse = {};

    // Create and append to DOM.
    var xcanvas = jQuery(
      '<canvas></canvas>'
    ).attr(
      "id",
      settings.particle_emitter.canvas
    );
    jQuery(settings.particle_emitter.container_id).append(xcanvas);

    // Initialize canvas.
    var canvas = document.getElementById(settings.particle_emitter.canvas);
    var ctx = canvas.getContext("2d");

    // Canvas dimensions.
    var canvas_width = settings.particle_emitter.canvas_width;
    var canvas_height = settings.particle_emitter.canvas_height;
    if (canvas_width == 0) {
      canvas_width = jQuery(settings.particle_emitter.container_id).width();
    }
    if (canvas_height == 0) {
      canvas_height = jQuery(settings.particle_emitter.container_id).height();
    }
    canvas.width = canvas_width;
    canvas.height = canvas_height;

    function mouse_tracking(e) {
      var pos = findPos(this);
      var x = e.pageX - pos.x;
      var y = e.pageY - pos.y;
      mouse.x = x;
      mouse.y = y;
    }

    function findPos(obj) {
      var curleft = 0, curtop = 0;
      if (obj.offsetParent) {
      do {
        curleft += obj.offsetLeft;
        curtop += obj.offsetTop;
      } while (obj = obj.offsetParent);
        return { x: curleft, y: curtop };
      }
      return undefined;
    }

    function particle_color() {

      if (settings.particle_emitter.particles_fill_type == "random") {
        this.r = Math.round(Math.random() * 255);
        this.g = Math.round(Math.random() * 255);
        this.b = Math.round(Math.random() * 255);
      }
      else if (settings.particle_emitter.particles_fill_type == "fixed") {
        var red = parseInt(settings.particle_emitter.particles_color_hex.substring(1,3),16);
        var green = parseInt(settings.particle_emitter.particles_color_hex.substring(3,5),16);
        var blue = parseInt(settings.particle_emitter.particles_color_hex.substring(5,7),16);
        this.r = red;
        this.g = green;
        this.b = blue;
      }
    }

    function particle() {

      if (settings.particle_emitter.emitter_position_type == "center" || (settings.particle_emitter.emitter_position_type == "mouse" && (!mouse.x || !mouse.y))) {
        this.location = {x: canvas_width / 2, y: canvas_height / 2};
      }
      else if (settings.particle_emitter.emitter_position_type == "mouse" && mouse.x && mouse.y) {
        this.location = {x: mouse.x, y: mouse.y};
      }
      else if (settings.particle_emitter.emitter_position_type == "offset") {
        this.location = {x: parseInt(settings.particle_emitter.emitter_offset_x), y: parseInt(settings.particle_emitter.emitter_offset_y)};
      }
      // speed - x range = -2.5 to 2.5
      // speed - y range = -15 to -5 to = up
      // this.speed = {x: -2.5 + Math.random() * 5, y: -15 + Math.random() * 10};
      this.speed = {x: -2.5 + Math.random() * 5, y: -2.5 + Math.random() * 5};

      if (settings.particle_emitter.particle_type == "circle") {
        if (settings.particle_emitter.particle_radius == "random") {
          this.radius = parseInt(settings.particle_emitter.particle_radius_min) + Math.random() * (parseInt(settings.particle_emitter.particle_radius_max) - parseInt(settings.particle_emitter.particle_radius_min));
        }
        else if (settings.particle_emitter.particle_radius == "fixed") {
          this.radius = parseInt(settings.particle_emitter.particle_radius_fix);
        }
      }
      else if (settings.particle_emitter.particle_type == "rectangle") {
        this.rect_width = parseInt(settings.particle_emitter.particle_rect_width);
        this.rect_height = parseInt(settings.particle_emitter.particle_rect_height);
      }
      else if (settings.particle_emitter.particle_type == "image") {
        this.imageObj = new Image();
        this.imageObj.onload = function() {
          ctx.drawImage(this.imageObj, this.location.x, this.location.y);
        };
        this.imageObj.src = settings.particle_emitter.particle_image_set[0];
      }
      else if (settings.particle_emitter.particle_type == "text") {
        ctx.font = settings.particle_emitter.particle_text_style;
        ctx.fillText(settings.particle_emitter.particle_text_string, this.location.x, this.location.y);
      }

      // life range = 20-30
      // this.life = 20 + Math.random() * 10;
      if (settings.particle_emitter.particle_life == "random") {
        this.life = parseInt(settings.particle_emitter.particle_life_min) + Math.random() * (parseInt(settings.particle_emitter.particle_life_max) - parseInt(settings.particle_emitter.particle_life_min));
      }
      else if (settings.particle_emitter.particle_life == "fixed") {
        this.life = parseInt(settings.particle_emitter.particle_life_fix);
      }
      this.lifetime = this.life;
      // colors
      var rgb = new particle_color();
      this.r = rgb.r;
      this.g = rgb.g;
      this.b = rgb.b;
    }

    function particle_emission() {

      // Ctx bg settings for current frame.
      ctx.globalCompositeOperation = settings.particle_emitter.canvas_composite_type;
      ctx.fillStyle = settings.particle_emitter.canvas_bgcolor;
      ctx.fillRect(0, 0, canvas_width, canvas_height);

      // Ctx particle settings for current frame.
      ctx.globalCompositeOperation = settings.particle_emitter.particle_composite_type;

      for (var i = 0; i < particles.length; i++) {
        var p = particles[i];
        ctx.beginPath();
        p.opacity = Math.round(p.lifetime / p.life * 100) / 100;

        ctx.fillStyle = 'rgba(' + p.r + ', ' + p.g + ', ' + p.b + ', ' + p.opacity + ')';

        if (settings.particle_emitter.particle_type == "circle") {
          if (settings.particle_emitter.particles_fill_style == "gradient") {
            // @todo gradient for circle  and rectangle, else nothing...
            var gradient = ctx.createRadialGradient(p.location.x, p.location.y, 0, p.location.x, p.location.y, p.radius);
            gradient.addColorStop(0, "rgba(" + p.r + ", " + p.g + ", " + p.b + ", " + p.opacity + ")");
            gradient.addColorStop(0.5, "rgba(" + p.r + ", " + p.g + ", " + p.b + ", " + p.opacity + ")");
            gradient.addColorStop(1, "rgba(" + p.r + ", " + p.g + ", " + p.b + ", 0)");
            ctx.fillStyle = gradient;
          }
          ctx.arc(p.location.x, p.location.y, p.radius, Math.PI * 2, false);

          if (settings.particle_emitter.particle_radius_transform == "shrink") {
            p.radius--;
          }
          else if (settings.particle_emitter.particle_radius_transform == "grow") {
            p.radius++;
          }
        }
        else if (settings.particle_emitter.particle_type == "rectangle") {
          ctx.rect(p.location.x, p.location.y, p.rect_width, p.rect_height);

          if (settings.particle_emitter.particle_rect_transform == "shrink") {
            p.rect_width--;
            p.rect_height--;
          }
          else if (settings.particle_emitter.particle_rect_transform == "grow") {
            p.rect_width++;
            p.rect_height++;
         }
        }
        else if (settings.particle_emitter.particle_type == "image") {
          ctx.globalAlpha = p.opacity;
          ctx.drawImage(p.imageObj, p.location.x, p.location.y);
        }
        else if (settings.particle_emitter.particle_type == "text") {
          ctx.font = settings.particle_emitter.particle_text_style;
          ctx.fillText(settings.particle_emitter.particle_text_string, p.location.x, p.location.y);
        }
        ctx.fill();

        p.lifetime--;

        p.location.x += p.speed.x;
        p.location.y += p.speed.y;

        if (p.lifetime < 0 || p.radius < 0) {
          particles[i] = new particle();
        }
      }
    }

    // Create particles.
    for (var i = 0; i < settings.particle_emitter.particle_count; i++) {
      particles.push(new particle());
    }

    // Mouse tracking.
    if (settings.particle_emitter.emitter_position_type == "mouse") {
      canvas.addEventListener('mousemove', mouse_tracking, false);
    }

    setInterval(
      particle_emission,
      parseInt((1 / settings.particle_emitter.frame_rate) * 1000)
    );
  }
};
