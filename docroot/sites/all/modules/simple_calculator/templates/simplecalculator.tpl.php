<?php
/**
 * @file
 * Implementation for simplecalculator.
 */
?>
<html>
    <head></head>
    <body>
        <h3>Simple Calculator</h3>
        <br/>
        <form Name="calc">
            <table border=2  style="width: 30%;">
                <tr>
                    <td colspan=4><input type=text Name="display" id="calc-simple"></td>
                <span id="errmsg" style="color:red;"></span>
                </tr>
                <tr>
                    <td><input style="width: 100%;" type=button value="0" OnClick="calc.display.value += '0'"></td>
                    <td><input style="width: 100%;" type=button value="1" OnClick="calc.display.value += '1'"></td>
                    <td><input style="width: 100%;" type=button value="2" OnClick="calc.display.value += '2'"></td>
                    <td><input style="width: 100%;" type=button value="+" OnClick="calc.display.value += '+'"></td>
                </tr>
                <tr>
                    <td><input style="width: 100%;" type=button value="3" OnClick="calc.display.value += '3'"></td>
                    <td><input style="width: 100%;" type=button value="4" OnClick="calc.display.value += '4'"></td>
                    <td><input style="width: 100%;" type=button value="5" OnClick="calc.display.value += '5'"></td>
                    <td><input style="width: 100%;" type=button value="-" OnClick="calc.display.value += '-'"></td>
                </tr>
                <tr>
                    <td><input style="width: 100%;" type=button value="6" OnClick="calc.display.value += '6'"></td>
                    <td><input style="width: 100%;" type=button value="7" OnClick="calc.display.value += '7'"></td>
                    <td><input style="width: 100%;" type=button value="8" OnClick="calc.display.value += '8'"></td>
                    <td><input style="width: 100%;" type=button value="x" OnClick="calc.display.value += '*'"></td>
                </tr>
                <tr>
                    <td><input style="width: 100%;" type=button value="9" OnClick="calc.display.value += '9'"></td>
                    <td><input style="width: 100%;" type=button value="C" OnClick="calc.display.value = ''"></td>
                    <td><input style="width: 100%;" type=button value="=" OnClick="calc.display.value = eval(calc.display.value)"></td>
                    <td><input style="width: 100%;" type=button value="/" OnClick="calc.display.value += '/'"></td>
                </tr>
            </table>
        </form>
    </body>
</html>
