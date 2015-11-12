/**
 * @file
 * Provides function to format entered time.
 *
 * This javascript file is used by the racetime module
 * to format the entered value of a racetime field element
 */

/**
 * Format the entered time into proper time format hh:mm:ss.000.
 *
 * This function formats the entered value of element f
 * into a valid time string, using the number of decimals
 * provided to determine the valid time string.
 *
 * @param f
 * @param numDecimals
 *
 * @return string
 */
function formatTimeField(f,numDecimals){
   var theTime = f.value;

  // Stop the script if no time was entered.
  if(!theTime) {
    return;
  }

  // Check if the user entered a time as m:ss, with no fraction of a second.
  // If so, the time will be updated to include the fraction.
  if(theTime.match(/^\d?\d?:?\d?\d:\d\d$/)) {
    theTime += '.' + Array(numDecimals + 1).join(0)
  }
  // Remove any periods and colons from theTime using a regular expression
  // the regular expression is
  //
  //    /:|\./gi
  //
  // The regular expression is delimited by the two forward slashes(//)
  // We wish to remove any colons (:) or periods (.), so we put both characters
  // into the regular expression separated by the OR (|) operator. The
  // backslash (\) in front of the period is there because the period has a
  // special meaning in regular expressions, so it must be escapted.  The two
  // letters after the final forward slash (gi) indicate a global search,
  // meaning to search for the characters more then once, and a case
  // insensitive search
  //
  // We're removing these "valid" characters because we'll reinsert them later.
  // Since entered times may only have numbers, a colon and a period, these are
  // removed to determine if the remaining characters are numbers.
  theTime = theTime.replace(/:|\./gi,"");

  // Create a regular expression that will test for non numeric characters.
  var regEx = new RegExp("\\D");
  // If the number has any non numeric characters in it, then
  // an invalid time was entered.
  if(regEx.test(theTime)) {
    // Display an alert that the time is invalid.
    alert("Invalid Time");

    // Set focus to the form field.
    f.focus();

    return false;
  }
  // If the length of the time entered is less then the number of decimals
  // plus one, format the time a little differently.
  else if(theTime.length <= numDecimals + 1) {
    theTime = theTime / Math.pow(10,numDecimals);
  }
  // Otherwise, format the time.
  else {
    // Reverse the entered time for easy splitting.
    var reverseTime = theTime.split("").reverse().join("");

    // Get the decimal portion of the time by getting
    // the first numDecimals of the reversed time.
    var decimalsPart = reverseTime.slice(0, numDecimals);
    decimalsPart = decimalsPart.split("").reverse().join("");

    // Get the seconds part by getting the next 2 decimals.
    var secondsPart = reverseTime.slice(numDecimals, 2 + numDecimals);
    secondsPart = secondsPart.split("").reverse().join("");

    // Get the minutes time by getting the next one or two numbers
    // excluding the last part (the seconds and decimal portion).
    // If the time is less then minutesPart will be empty.
    var minutesPart = reverseTime.slice(numDecimals + 2,numDecimals + 4);

    // If minutesPart is not empty, then format it to include
    // the colon after the minute.
    if(minutesPart) {
      minutesPart = minutesPart.split("").reverse().join("");
      minutesPart = minutesPart + ":";
    }

    // Get the hours part of the time by getting the remaining part of the
    // string.
    var hoursPart = reverseTime.slice(numDecimals + 4);
    if(hoursPart) {
      hoursPart = hoursPart.split("").reverse().join("");
      hoursPart = hoursPart + ":";
    }

    theTime = hoursPart + minutesPart + secondsPart + "." + decimalsPart;
  }

  // Set the form field value to theTime.
  f.value = theTime;

  return true;
}
