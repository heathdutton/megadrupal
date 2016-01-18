/**
 * @class DrupalExt.model.DrupalModel
 * @extends Ext.data.Model
 * 
 * <p>
 * The DrupalModel class provides a Model with some custom functions to
 * facilitate integration of Drupal with Ext.
 * </p>
 * 
 * <p>
 * <em>Field synchronisation:</em> This function allows keeping specific field
 * values in sync with each other. The fieldSyncMap property contains a list of
 * field to field mappings containing the names of fields whose values should be
 * kept in sync. This property is generally defined in sub-classes (since in
 * most cases we want the same behaviour across all instances of the same
 * Model), but can also be set in the config options for individual instances.
 * Member names should be field names to monitor for changes, values should be
 * field names to update when the corresponding field referred to by the member
 * name changes. When a field value is set via the set() function and the field
 * name is a member of the fieldSyncMap Object, then the field whose name is the
 * value of this member will be updated with the same value (via the set()
 * function defined on Ext.data.Model). "Setting" cycles do not occur when the
 * fieldSyncMap contains field to field mappings that are opposites. If the
 * set(fieldName, value) function is called with the fieldName argument being a
 * map of field names to values, and fieldSyncMap contains a pair of opposite
 * definitions, and both values are specified in the fieldName argument, then if
 * one of the specified values == null then it will be ignored (this is most
 * useful when all the data for a Model is loaded).
 * </p>
 */

Ext.define('DrupalExt.model.DrupalModel', {
  extend: 'Ext.data.Model',
  alternateClassName: 'model.DrupalModel',
  
  /**
   * @property {Object} fieldSyncMap
   * Contains a list of field to field mappings containing the names of fields 
   * whose values should be kept in sync.
   */
  fieldSyncMap: {},
  
  /**
   * Contains one of the pairs of each pair of opposite definitions in 
   * fieldSyncMap.
   */
  fieldSyncMapOpposites: {},
  /**
   * Contains the definitions that don't have an opposite definition in 
   * fieldSyncMap.
   */
  fieldSyncMapNonOpposites: {},
  
  /**
   * Creates the DrupalModel.
   * @param {Object} config (optional) Config object.
   */
  constructor: function(config) {
    if ('fieldSyncMap' in config) {
      this.fieldSyncMap = config.fieldSyncMap;
    }
    
    // Sort field mapping definitions into pairs of opposites and unique.
    // This is used to help resolve conflicts when multiple field values are 
    // set together.
    for (var field1 in this.fieldSyncMap) {
      var field2 = this.fieldSyncMap[field1];
      // If there is an opposite definition.
      if (field2 in this.fieldSyncMap && this.fieldSyncMap[field2] == field1) {
        // If we haven't yet recorded this pair.
        if (!(field2 in this.fieldSyncMapOpposites)) {
          this.fieldSyncMapOpposites[field1] = field2;
        }
      }
      else { // It as a unique definition.
        this.fieldSyncMapNonOpposites[field1] = field2;
      }
    }
    
    this.callParent(arguments);
  },
  
  
  /**
   * Sets the given field to the given value, marks the instance as dirty.
   * If the specified field names are members of fieldSyncMap then the 
   * corresponding fields will also have their values updated.
   * @param {String/Object} fieldName The field to set, or an object containing key/value pairs
   * @param {Object} value The value to set
   */
  set: function(fieldName, value) {
    // If we're passed an Object mapping field names to values, check for and 
    // try to resolve conflicts between mapping definitions in 
    // fieldSyncMap and setting both fields at once in this request. 
    if ((arguments.length == 1 && Ext.isObject(fieldName))) {
      // Try to resolve conflicts involving pairs of opposite field sync 
      // definitions first.
      for (var field1 in this.fieldSyncMapOpposites) {
        var field2 = this.fieldSyncMapOpposites[field1];
        // If both fields are set in fieldName and have different values.
        if (field1 in fieldName && field2 in fieldName && fieldName[field1] !== fieldName[field2]) {
          // If one of them is null (false, 0, empty), remove it.
          if (fieldName[field1] == null || fieldName[field1] == "" || fieldName[field1] == "0") {
            delete fieldName[field1];
          }
          else if (fieldName[field2] == null || fieldName[field2] == "" || fieldName[field2] == "0") {
            delete fieldName[field2];
          }
          // Else there's nothing sensible we can do.
        }
      }
      
      // Try to resolve any remaining conflicts.
      for (var field1 in this.fieldSyncMapNonOpposites) {
        var field2 = this.fieldSyncMapNonOpposites[field1];
        // If both fields are set in fieldName.
        if (field1 in fieldName && field2 in fieldName) {
          // Prioritise the master field.
          delete fieldName[field2];
        }
      }
    }
    
    this.callParent(arguments);
    
    // If we're passed a field name and value.
    if (arguments.length == 2 && !Ext.isObject(fieldName)) {
      // If this field is in the field sync map update the corresponding field.
      if (fieldName in this.fieldSyncMap) {
        this.callParent([this.fieldSyncMap[fieldName], value]);
      }    
    }
  }
});