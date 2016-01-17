# Commerce Reports: Stock

This submodule of Commerce Reports provides insight to current stock levels. Using sales history, the module works to provide an estimation to a product's stock lifetime based on its current stock value.

Note: there are assumptions made you are using the default *commerce_stock* field provided by the Commerce Stock module. In reality this field could be changed out for any integer field.

### Explanation of report values

Reports are based off of a start time. This is either the product's creation time, or the value specified as the history period value (defaults to -3 months.)

**Stock:** The product's current stock value, based on the field *commerce_stock*.

**Monthly sales:** Total sales divided by period of time since history period, by month.

**Weekly sales:** Total sales divided by period of time since history period, by week.

**Stock lifetime:** Estimated amount of days the stock will last, based on weekly sales.
