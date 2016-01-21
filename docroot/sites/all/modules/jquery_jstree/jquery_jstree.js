(function ($) {
  Drupal.behaviors.jstree = {
    attach: function (context, settings) {
      // Initialize any jsTree defined from PHP
      if ($.isPlainObject(settings.jstree)) {
        $.each(settings.jstree, function (id, options) {
            if (settings.jstree.hasOwnProperty(id)) {
              // Retrieve the tree element
              var tree = $('#'+id+':not(.jstree)', context);
              if (tree.length) {
                // Pre-process options
                options.core.data.data = function (node) {
                  return {
                    'id' : node.id,
                    'default_value' : options.core.data.default_value
                  };
                }
                // Build the tree
                tree.jstree(options);
                var form = tree.parents('form');
                if (form.length) {
                  // Sync. tree-node selection to hidden fields in parent form.
                  var inputName = tree.data('name');
                  tree.on('changed.jstree', function (e, data) {
                    form.find('input.selected-node').remove();
                    for(i = 0, j = data.selected.length; i < j; i++) {
                      var node = data.instance.get_node(data.selected[i]);
                      $('<input></input>').attr({
                        type: 'hidden',
                        name: inputName + '[' + i + ']',
                        value: node.id,
                        'class': 'selected-node'
                      }).insertAfter(tree);
                    }
                    tree.trigger('dom-changed.jstree');
                  });
                  // Serialize the parent form when the tree triggers an ajax event
                  if (typeof Drupal.ajax === 'function' && Drupal.ajax.hasOwnProperty(id)) {
                    Drupal.ajax[id].beforeSerialize = function(element, options) {
                      $.each(form.serializeArray(), function(id, param) {
                        options.data[param.name] = param.value;
                      });
                      Drupal.ajax.prototype.beforeSerialize.apply(this, arguments);
                    };
                  }
                }
              }
            }
        });
      }
    },
    detach: function (context) {
      // Destroy any (automatically created) jsTree in the detached context
      if ($.isPlainObject(Drupal.settings.jstree)) {
        $.each(Drupal.settings.jstree, function (id, options) {
          if (Drupal.settings.jstree.hasOwnProperty(id)) {
            var tree = $('#'+id, context);
            tree.jstree('destroy');
          }
        });
      };
    }
  };
}(jQuery));
