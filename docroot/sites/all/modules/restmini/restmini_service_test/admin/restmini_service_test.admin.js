
/**
 * Check/unckeck all (child) tests of a module.
 *
 * @param {element} that
 */
var restminiServiceTestCascadeDown = function(that) {
  var chkd = that.checked, chkbxs = jQuery('input[type="checkbox"]').get(), le = chkbxs.length, i, elm, pattern = that.name;
  pattern = pattern.substr(0, pattern.length - 1) + ':';
  for (i = 0; i < le; i++) {
    elm = chkbxs[i];
    if (elm.name.indexOf(pattern) === 0) {
      elm.checked = chkd ? 'checked' : false;
    }
  }
},
/**
 * Uncheck (parent) module of a test.
 *
 * @param {element} that
 */
restminiServiceTestCascadeUp = function(that) {
  var chkbxs = jQuery('input[type="checkbox"]').get(), le = chkbxs.length, i, elm, pattern = that.name;
  pattern = pattern.substr(0, pattern.indexOf(':')) + ']';
  for (i = 0; i < le; i++) {
    elm = chkbxs[i];
    if (elm.name === pattern) {
      elm.checked = false;
      break;
    }
  }
};
