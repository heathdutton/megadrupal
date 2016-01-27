Ext.define('DrupalExt.view.model.FieldValues', {
  extend: 'Ext.panel.Panel',
  alias : 'widget.fieldValues',
  id: 'FieldValues',
  autoScroll: true,
  
  initComponent: function() {
    this.items = [ {
      xtype: 'panel',
      html: "<p>Hover over a row to display all field values for the Model in this panel.</p>",
      width: 500
    } ];
      
    this.callParent(arguments);
  },
  
  showModel: function(model) {
    var html = '<table><tr><th>Data field</th><th>Value</th></tr>';
    for (field in model.data) {
      html += '<tr><td>' + field + ': </td><td>' + model.data[field] + '</td></tr>';
    }
    html += '</table>';
    this.down('panel').update(html);
  }
});