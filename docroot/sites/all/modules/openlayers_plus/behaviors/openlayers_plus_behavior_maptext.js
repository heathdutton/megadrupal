(function($, Drupal) {

  Drupal.behaviors.openlayers_plus = {
      attach : function(context, setting) {
        var toggle_setting;
        $('a.toggle_button').click(function (e) {
          e.stopImmediatePropagation();
          if($.cookie('block_titles') != null && $.cookie('block_titles').indexOf(',') == -1) {

            $(this).toggleClass("down");

            $('div#' + $(this).attr('id')).parent().toggle();
            if ($(this).attr('class').indexOf('down') != -1) {
              $.cookie($(this).html(), 'down');

            }else {
              $.cookie($(this).html(), 'up');

            }
            }else {

            var blocks = $.cookie('block_titles').split(',');

            for(var y = 0; y < blocks.length; y++) {
              if(blocks[y] != "" && blocks[y].indexOf('#') === -1 && blocks[y].indexOf('context') === -1) {
                if(blocks[y] == $(this).attr('id')) {
                  $('div#' + blocks[y]).parent().toggle();
                  $('a#' + blocks[y]).toggleClass("down");
                }else if (
                  $('div#' + blocks[y]).is(":visible")) {
                  $('a#' + blocks[y]).toggleClass("down");
                  $('div#' + blocks[y]).parent().toggle();
                }
                if($('a#' + blocks[y]).attr('class').indexOf('down') != -1) {
                  $.cookie($('a#' + blocks[y]).html(), 'down');

                }else {
                  $.cookie($('a#' + blocks[y]).html(), 'up');

                }
                }
            }
            }

        });

        $('.maptext a.popup-close').click(function(e){
          e.stopImmediatePropagation();
          var klass = $(this).parent().attr('id');
          $('a#' + klass).toggleClass("down");
          $("div#" + klass).parent().toggle();
          $.cookie($('a#' + klass).html(), 'down');
          return false;
        });
        $('.bntitle').click(function(e){
          e.stopImmediatePropagation();
         $(".bnContainer").slideToggle('slow');
         });
        
        if(setting.openlayers_plus.drag == 1){
          $('.btns').draggable( {
            containment: $('.openlayers-views-map'),
            drag: function( event, ui ) {
              $(this).css('cursor','pointer');
            },
            stop: function( event, ui ) {
              $(this).css('cursor','default');
              var Stoppos = [];
              Stoppos = ui.position;
              $.cookie("pos-left", Stoppos['left']);
              $.cookie("pos-top", Stoppos['top']);
              
            }
            
          });
        }
        else {
          $.cookie("pos-left", null);
          $.cookie("pos-top", null);
        }
        
        var data = $(context).data('openlayers');
        if (data && data.map.behaviors.openlayers_plus_behavior_maptext) {
          var ctrl;
          var map = data.openlayers;
          toggle_setting = setting.openlayers_plus.Toggle;
          button_title = setting.openlayers_plus.Btn_Titles;
          collapse = setting.openlayers_plus.collapse;

          regions = setting.openlayers_plus.regions;
          controls = new Array();
          for (region in regions) {
            var region_toggles = new toggles();

            for (block in regions[region]) {
              block_toggle = new togle(
                  block,
                  regions[region][block].title,
                  regions[region][block].title,
                  regions[region][block].markup,
                  0,0,0,0,
                  region);
              region_toggles.addElement(block_toggle);
            }
            region_toggles.showControlHTML(map, toggle_setting);
            region_toggles.showButtonHTML(map, toggle_setting, button_title, collapse);

          }

        }
      }
  };

  Object.size = function(obj) {
    var size = 0, key;
    for (key in obj) {
      if (obj.hasOwnProperty(key)) {
        size++;
      }
    }
    return size;
  };

  function togle(id,cTitle, blabel, ctrl, lft, rgt, tp, btm, Region){
    this.iD = id;
    this.buttonlabel = blabel;
    this.controlTitle = cTitle;
    this.controlcontent = ctrl;

    this.left = lft;
    this.top = tp;
    this.right = rgt;
    this.bottom = btm;
    this.region = Region;

    this.draw = function(content, type, indx) {
      var element = new mtext();

      element.html = content;
      if(type == 'btn') {
        element.button = true;
        element.maptext = false;
      }else if(type == 'maptext') {
        element.button = false;
        element.maptext = true;
        element.index = indx;
      }
      
      if($.cookie("pos-left") != null && $.cookie("pos-top") != null) {
        element.left = $.cookie("pos-left");
        element.top = $.cookie("pos-top");
      }
      else {
        element.left = this.left;
        element.top = this.top;
      }
    
      element.identifier = this.iD;
      element.bottom = this.bottom;
      element.right = this.right;
      
      
      return element;
    }

  }

  function toggles() {
    this.elements = [];

    this.addElement = function(instance) {
      if(this.elements.length == 0){
        this.elements[this.elements.length] = instance;
      }else if(this.elements.length >= 1){
        this.elements[this.elements.length] = instance;
      }

    }

    this.showControlHTML = function(map, toggl_menu) {
      var mapHTML;
      var toggleObject;

      for (var toggleObjectkey in this.elements) {
        toggleObject = this.elements[toggleObjectkey];
        mapHTML = toggleObject.draw('<div class="maptext" id="' + toggleObject.iD + '">' +
            '<a class="popup-close" href="">X</a> <h2 class="popup-title"><a>' + toggleObject.controlTitle + '</a></h2>' +
            '<div class="popup-content">' + toggleObject.controlcontent + '</div>' + '<div class="popup-pager"><ul class="links" style="display: none;">' + '<li><a class="prev disabled" href="#prev">Prev</a></li>' + '<li><a class="next" href="#next">Next</a></li>' + '</ul><span class="count">1 / 0</span></div>' + '</div>','maptext',toggleObjectkey);
        if (toggleObject.controlTitle != "") {
          map.addControl(mapHTML);
          if ($.cookie(toggleObject.controlTitle) == 'down') {
            $('div#' + toggleObject.iD).parent().toggle();
          }
          else if ($.cookie(toggleObject.controlTitle) == undefined) {
            $('div#' + toggleObject.iD).parent().hide();
          }

        }

      }

    }

    this.showButtonHTML = function(map, toggl_menu, btntitle, colapse) {

      var bnGroup = "";
      var hideAnchor = [];
      var toggleobject;
      var block_title = '';

      for (var x = 0; x < toggl_menu.length; x++) {
        block_title = toggl_menu[x] + ',' + block_title;
      }
      
      $.cookie('block_titles', block_title);

      for (var toggleobjectkey in this.elements) {
        toggleobject = this.elements[toggleobjectkey];
        if (toggleobject.buttonlabel != "") {
          bnGroup = bnGroup + '<div class="' + toggleobject.iD + '"><a class="toggle_button" id="' + toggleobject.iD + '" title="' + toggleobject.buttonlabel + '">' + toggleobject.buttonlabel + '</a></div>';
          if ($.cookie(toggleobject.buttonlabel) == 'down') {
            hideAnchor.push(toggleobject.iD);
          }
        }
      }

      if (colapse[toggleobject.region] === 1) {
        group = "<a><b><h3 class='bntitle'>" + btntitle[toggleobject.region] + "</h3></b></a><div class='bnContainer'>" + bnGroup + "</div>";
      }
      else {
        group = "<div class='bnContainer'>" + bnGroup + "</div>";
      }

      if (bnGroup != "") {
        map.addControl(toggleobject.draw(group,'btn',toggleobjectkey));
      }
      if ($.cookie(toggleobject.buttonlabel) == undefined) {
        $('a.toggle_button').toggleClass('down');
      }
      else {
        for (var xkey in hideAnchor) {
          var x = hideAnchor[xkey];
          $('div.bnContainer').find('a#' + x).toggleClass('down');

        }
      }
    }
  }

})(jQuery, Drupal);

mtext = OpenLayers.Class(OpenLayers.Control.Attribution, {

  separator: ", ",
  html: "",

  button:"",
  maptext:"",
  index:"",
  top:"",
  right:"",
  bottom:"",
  left:"",
  identifier:"",

  destroy: function() {
    this.events.un({
      "removelayer": this.updateAttribution,
      "addlayer": this.updateAttribution,
      "changelayer": this.updateAttribution,
      "changebaselayer": this.updateAttribution,
      scope: this
    });

    OpenLayers.Control.prototype.destroy.apply(this, arguments);
  },
  draw: function() {
    OpenLayers.Control.prototype.draw.apply(this, arguments);

    this.events.on({
      'changebaselayer': this.updateAttribution,
      'changelayer': this.updateAttribution,
      'addlayer': this.updateAttribution,
      'removelayer': this.updateAttribution,
      scope: this
    });

    if (this.button) {
      this.div.classList.add('btns');
    }
    else if (this.maptext) {
      this.div.classList.add('maptext_' + this.index + '_' + this.identifier);
    }
    
    this.updateAttribution();

    return this.div;
    },
  updateAttribution: function() {
    if (this.map && this.map.layers) {
      this.div.innerHTML = this.html;
      this.div.style.left = this.left+'px';
      this.div.style.top = this.top+'px';
    }
    
  },

  CLASS_NAME: "OpenLayers.Control.Maptext"
});

