/**
 * @file
 * Menu Tree Compare javascript functionality.
 */

(function($) {
  Drupal.behaviors.menu_tree_compare = {
    attach: function(context) {

      $.jstree.plugins.nohover = function () { this.hover_node = jQuery.noop; };
      $.jstree.plugins.noselect = function () { this.select_node = jQuery.noop; };

      var jstree_settings = {
        "plugins" : [ "nohover", "noselect"]
      };

      // Set up left tree.
      $('#menu_tree_compare-left').jstree(jstree_settings);

      // Set up right tree.
      $('#menu_tree_compare-right').jstree(jstree_settings);

      // Get the new height of the container.
      var left_height = $('#menu_tree_compare-left').height();
      var right_height = $('#menu_tree_compare-right').height();
      // Take the height of the tallest out of (left, right) and add 20 pixels.
      var container_height =  Math.max.apply( Math, [ left_height, right_height ] ) + 20;
      // Set the weight.
      $('#menu_tree_compare-container').height(container_height);

      // Set up connectors.
      jsPlumb.ready(function() {

        // Common settings among all connections.
        var common = {
          anchors : ["Continuous", "Continuous"],
          endpoints : ["Blank", "Blank"],
          overlays : [[ "Arrow", { location:1 } ], [ "Arrow", { location:1 } ]],
          paintStyle : { lineWidth:4, strokeStyle:"#f0f0f0" },
          // When clicking on the connector, scroll to the target.
          hoverPaintStyle : { strokeStyle:"#5a5a5a", lineWidth:4 },
          endpointHoverStyle : { fillStyle:"#00ff00" }
        };

        // Make the trees draggable.
        jsPlumb.draggable($("#menu_tree_compare-left"));
        jsPlumb.draggable($("#menu_tree_compare-right"));

        // Set up container.
        jsPlumb.setContainer("menu_tree_compare-container");

        // Make sure we hide the connections of the children when closing a
        // node.
        $('.jstree').on("after_close.jstree", function (e, data) {
          for (var child in data.node.children_d) {
            jsPlumb.hide(data.node.children_d[child] + '_anchor');
          }
          jsPlumb.repaintEverything();
        });

        // Show the connections of the children again.
        $('.jstree').on("after_open.jstree", function (e, data) {
          jsPlumb.repaintEverything();
        });

        // After closing and opening a node, its child connections will be
        // "broken", ie pointing to the wrong source/target. We will detach
        // them, change the IDs, and create new ones.
        $('.jstree').on("open_node.jstree", function (e, data) {
          for (var child in data.node.children_d) {

            // We don't know whether this is in the source or the target, so
            // look in both.
            var broken_connections_source = jsPlumb.getConnections({
              source : data.node.children_d[child] + '_anchor',
            });
            var broken_connections_target = jsPlumb.getConnections({
              target : data.node.children_d[child] + '_anchor',
            });
            var broken_connections = broken_connections_source.concat(broken_connections_target);

            for (var conn in broken_connections) {
              // Remove the broken connection.
              var source_name = broken_connections[conn].sourceId.replace("_anchor", "");
              var target_name = broken_connections[conn].targetId.replace("_anchor", "");

              jsPlumb.detach(broken_connections[conn]);

              // Temporarily rename the source and the target.
              var new_source_name = String.fromCharCode(65 + Math.floor(Math.random() * 26)) + Math.floor(Math.random() * 1000000);
              var new_target_name = String.fromCharCode(65 + Math.floor(Math.random() * 26)) + Math.floor(Math.random() * 1000000);

              var source_node = $('.jstree-1').jstree(true).get_node(source_name);
              var target_node = $('.jstree-2').jstree(true).get_node(target_name);

              // We change the ID because using the old ID means the connectors
              // go to the top left of the screen.
              $('#' + source_name + '_anchor').attr('id', new_source_name + '_anchor');
              $('.jstree-1').jstree(true).set_id(source_node, new_source_name);
              source_node.li_attr.id = new_source_name;
              source_node.a_attr.id = new_source_name + '_anchor';

              $('#' + target_name + '_anchor').attr('id', new_target_name + '_anchor');
              $('.jstree-2').jstree(true).set_id(target_node, new_target_name);
              target_node.li_attr.id = new_target_name;
              target_node.a_attr.id = new_target_name + '_anchor';

              // Make a new connection.
              addConnection([{
                source : new_source_name + '_anchor',
                target : new_target_name + '_anchor'
              }]);
            }
          }
        });

        // Set up connection data for left & right trees.
        var jstree_left = {};
        var jstree_right = {};

        // Set up left (source) tree.
        $('.jstree-1 li').each(function(){
          var nid = $(this).attr('id').split('-')[2];
          if (nid != 'none') {
            if (!jstree_left[nid]) {
              jstree_left[ nid ] = [];
            }
            jstree_left[nid].push($(this).attr('id') + '_anchor');
          }
        });

        // Set up right (target) tree.
        $('.jstree-2 li').each(function(){
          var nid = $(this).attr('id').split('-')[2];
          if (nid != 'none') {
            if (!jstree_right[nid]) {
              jstree_right[nid] = [];
            }
            jstree_right[nid].push($(this).attr('id') + '_anchor');
          }
        });

        // Loop through left items in order to set connections from left to right.
        var connections = [];
        for (var nid in jstree_left) {
          for (var key in jstree_left[nid]) {
            // Look for matching nodes on the right.
            if (jstree_right[nid]) {
              for (var right_key in jstree_right[nid]) {
                connections.push({
                  source : $('#' + jstree_left[nid][key]),
                  target : $('#' + jstree_right[nid][right_key])
                });
              }
            }
          }
        }
        // Add the connections.
        addConnection(connections);

        function addConnection(connections) {
          // Loop through left anchors. Find matching anchors on the right.
          for (var id in connections) {
            var connection = jsPlumb.connect(connections[id], common);
            connection.bind("click", function(conn) {
              // On first click, scroll to the target. On second click, scroll
              // to the source.
              if (!conn['menu_tree_compare-clicks']) {
                conn['menu_tree_compare-clicks'] = 1;
              }
              else {
                conn['menu_tree_compare-clicks'] += 1;
              }
              if (conn['menu_tree_compare-clicks'] % 2) {
                var scroll_target = $("#" + conn.targetId);
              }
              else {
                var scroll_target = $("#" + conn.sourceId);
              }
              jQuery('html, body').animate({
                scrollTop: scroll_target.offset().top - 50
              }, 1000);

              // Remove the highlight class from the other elements.
              $('.jstree-container-ul a').not("#" + conn.sourceId).not("#" + conn.targetId).removeClass('menu_tree_compare-highlight');

              // Add the highlight class to the source & target.
              $("#" + conn.sourceId).addClass('menu_tree_compare-highlight');
              $("#" + conn.targetId).addClass('menu_tree_compare-highlight');

              // Repaint the connections as the highlighting changes the size
              // of the highlighted elements.
              jsPlumb.repaintEverything();
            });
          }
        }
      });
    }
  }
})(jQuery);
