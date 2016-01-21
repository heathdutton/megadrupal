<?php
/**
 * @file
 * Implementation for homeloan.
 */
?>
<html>
    <head></head>
    <body>
        <h3>Home Loan Calculator</h3>
        <br/>
    <center>
        <form name=calc method=POST>
            <table width=60% border=0>
                <tr>
                    <th bgcolor="#aaaaaa" width=50%><font color=blue>Description</font></th>
                    <th bgcolor="#aaaaaa" width=50%><font color=blue>Data Entry</font></th>
                </tr>
                <tr>
                    <td bgcolor="#eeeee">Loan Amount</td>
                    <td bgcolor="#aaeeaa" align=right>
                        <input type=text name=loan size=10 id="loan-amount">
                    </td>
                </tr>
                <tr>
                    <td bgcolor="#eeeee">Loan Length in Months</td>
                    <td bgcolor="#aaeeaa" align=right>
                        <input type=text name=months size=10 id="loan-length">
                    </td>
                </tr>
                <tr>
                    <td bgcolor="#eeeee">Interest Rate</td>
                    <td bgcolor="#aaeeaa" align=right>
                        <input type=text name=rate size=10 id="loan-tenature">
                    </td>
                </tr>
                <tr>
                    <td bgcolor="#eeeee">Monthly Payment</td>
                    <td bgcolor="#eeaaaa" align=right>
                        <em>Calculated</em>
                        <input type=text name=pay size=10 id="loan-payment" readonly>
                    </td>
                </tr>
                <tr>                    
                    <td bgcolor="#eeeeaa" align=center>
                        <input type=reset value=Reset id="homeloan-reset">
                    </td>
                    <td  bgcolor="#aaeeaa"align=center>
                        <input type=button id="homeloan-calc" value=Calculate>
                    </td>
                </tr>
            </table>                 
        </form>
    </center>
</body>
</html>
