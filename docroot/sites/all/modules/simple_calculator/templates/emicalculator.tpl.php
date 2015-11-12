<?php
/**
 * @file
 * Implementation for emicalculator.
 */
drupal_add_js(drupal_get_path('module', 'simple_calculator') . '/js/simple_calculator.js');
?>

<html>

    <head></head>
    <body>
        <h3>Loan EMI Calculator</h3>
        <form name="formval">
            <table cellpadding=3>
                <caption><b><u>EMI Calculator</u></b></caption>
                <span id="errmsg" style="color:red;"></span>
                <tr>
                    <td>Principal Amount :</td>
                    <td><input name="pr_amt" type="text" id="principal-amt"></td>
                </tr>
                <tr>
                    <td>Interest Rate :</td>
                    <td><input name="int_rate" type="text" id="int-rate"></td>
                </tr>
                <tr>
                    <td>Period (Months) :</td>
                    <td><input name="period" type="text" id="principal-period"></td>
                </tr>
            </table>
            <br><input type="button" name="calculate" value="Calculate" id="calculte-emi">
            <br>
            <table cellpadding=3>
                <tr>
                    <td>EMI :</td>
                    <td><input name="field1" type="text" id="output-emi" readonly></td>
                </tr>
                <tr>
                    <td>Interest Amount :</td>
                    <td><input name="field3" type="text" id="output-interest" readonly></td>
                </tr>
                <tr>
                    <td>Total Amount :</td>
                    <td> <input name="field2" type="text" id="output-total" readonly></td>
                </tr>
            </table>
            <div id="table-box"></div>     
            <br><input type="button" name="ptint" value="Print" id="print-emi">
        </form>
    </body>
</html>
