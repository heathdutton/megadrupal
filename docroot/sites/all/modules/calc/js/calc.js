/**
 * Display calc
 */
jQuery(document).ready(function() {
  jQuery("#calc-wrapper").show();
});

/**
 * Form variable to perform operations on calc form.
 */
var form = document.getElementById("calc-generate-calculator");

/**
 * To store last calculated value.
 */
var x = 0;

/**
 * To check the operation req. whether +,-,*,/ or =.
 */
var ops = "n";

/**
 * To check if result values needs to be reset or not.
 */
var token = 0;

/**
 * Function to return result for the operation performed on calc.
 */
function calc(op) {
  if (op == "1" || op == "2" || op == "3" || op == "4" || op == "5" || op == "6" || op == "7" || op == "8" || op == "9" || op == "0" || op == ".") {
    if (!token) {
      if (form.calc_result.value == 0) {
        form.calc_result.value = op;
      }
      else {
        form.calc_result.value = form.calc_result.value + op;
      }
    }
    else {
      form.calc_result.value = op;
      token = 0;
    }
    return;
  }

  if (op == "C") {
    form.calc_result.value = 0;

    x = 0;
    token = 0;
    return;
  }

  if (op == "%") {
    form.calc_result.value = form.calc_result.value / 100.0;

    token = 1;
    return;
  }

  if (op == "+/-") {
    form.calc_result.value = -form.calc_result.value;

    token = 1;
    return;
  }

  if (op == "1/x") {
    if (form.calc_result.value != 0) {
      form.calc_result.value = 1 / form.calc_result.value;
    }
    else {
      form.calc_result.value = 1 / form.calc_result.value;
    }

    token = 1;
    return;
  }


  if (op == "+" || op == "-" || op == "*" || op == "/" || op == "=") {
    token = 1;

    if (ops != "n") {
      if (ops == "+") {
        x = x -(- form.calc_result.value);
        form.calc_result.value = x;
      }

      if (ops == "-") {
        toFixedVal = 0;

        if (form.calc_result.value.indexOf(".") != -1) {
          toFixedVal = (x.length > form.calc_result.value.length) ? ((x.split(".")[1] > 0) ? x.split(".")[1].length : 0) : ((form.calc_result.value.split(".")[1] > 0) ? form.calc_result.value.split(".")[1].length : 0);
          x = x - form.calc_result.value;
          x = x.toFixed(toFixedVal);
        }
        else {
          x = x - form.calc_result.value;
        }

        form.calc_result.value = x;
      }

      if (ops == "/") {
        if (x > 0) {
          x = x / form.calc_result.value;
          form.calc_result.value = x;
        }
      }

      if (ops == "*") {
        if (x > 0) {
          x = x * form.calc_result.value;
          form.calc_result.value = x;
        }
      }
    }
    else {
      x = form.calc_result.value;
    }

    if (op != "=") {
      ops = op;
    }
    else {
      ops = "n";
    }

    return;
  }
}
