Ext.define('DrupalExt.controller.node.FieldValues', {
  extend: 'Ext.app.Controller',
  
  views: [ 'node.Grid', 'model.FieldValues' ],
  models: [ 'Node' ],
  stores: [ 'Node' ],
  
  init: function() {
    this.control({
      'nodeGrid' : {
        itemmouseenter: this.nodeHover 
      }
    });
    
    this.callParent(arguments);
  },
  
  nodeHover: function(view, model, item, index, event, eOpts) {
    Ext.getCmp('FieldValues').showModel(model);
  }
});

