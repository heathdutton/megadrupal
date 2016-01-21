/**
 * The application Viewport, defines the overall layout. 
 */
Ext.define('DrupalExt.view.Viewport', {
  extend: 'Ext.container.Viewport',
  layout: 'fit',

  initComponent: function() {
    this.items = [ {
      xtype: 'panel',
      layout: {
        type: 'hbox',
        align: 'stretch'
      },
      items: [ {
        xtype: 'nodeGrid',
        layout: 'fit',
        flex: 1
      },
      {
        xtype: 'fieldValues',
        layout: 'fit',
        flex: 1
      } ]
    } ];

    this.callParent();
  }
});