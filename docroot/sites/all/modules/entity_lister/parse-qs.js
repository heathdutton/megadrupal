/**
 * @file
 * Parse a query string.
 */

function parseQS(varName, qs) {

  var qStr = qs + '&';
  var regex = new RegExp('.*?[&\\?]' + varName + '=(.*?)&.*');
  var val = qStr.replace(regex, "$1");

  return val === qStr ? false : val;

}
