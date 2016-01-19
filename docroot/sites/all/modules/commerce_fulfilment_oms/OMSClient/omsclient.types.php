<?php

/**
 * @file This file maps the SOAP datatypes to PHP classes.
 */


/**
 * ViewStockLevels
 */
class ViewStockLevels {
  public $strB2BCustomerName;
  public $strB2BCustomerEmail;
  public $strB2BCustomerPassword;

  public $strSKU;
  public $dtmStartDate;
  public $strTimeZone;
}


/**
 * ExportOrders
 */
class ExportOrders {
  public $strB2BCustomerName;
  public $strB2BCustomerEmail;
  public $strB2BCustomerPassword;

  /**
   * @var Transaction type
   * SHP = Sales Order
   * RMA = Return Merchandise Authorization. This is a return document that can be changed.
   * RO = ?
   * PO = Purchase Order. This is a Purchase Order Document that is in its original version and has not been changed.
   * ASN = Advanced Shipment Notice. This is a version of the Purchase Order that can be changed.
   * CXL = Canceled/Voided order;
   */
  public $strTransactionType;

  /**
   * Multiple merchant orders can be passed to the method. To pass in multiple
   * merchant orders make sure the order numbers are separated a comma.
   * Example: “10001,10002”
   */
  public $strOrderID;
  public $strMerchantOrderID;

  /**
   * Specify this value only to return orders in a specific status. If you are
   * going to enter multiple statuses, each status needs to be separated a comma.
   *
   * For example, if you want all completed and voided orders then enter "Complete, Void".
   * If this parameter is left blank it will return orders in all statuses.
   */
  public $strOrderStatus;

  /**
   * If a date time is provided, the method will return only those orders that
   * have been updated and created since the date and time provided.
   */
  public $dtmOrdersFromDate;

  public $strTimeZone;
  public $strClientRoleName;
}


/**
 * CreateSalesOrder
 *
 * @see https://ws.ordermanagementsystems.com.au/omsordertaker/omsordertaker.asmx?op=CreateSalesOrder
 */
class CreateSalesOrder {
  public $LiveMode;
  public $order;
}


/**
 * SalesOrder
 *
 * This is a complex type defined by CreateSalesOrder. This class lists all the properties as required by
 * the SOAP WSDL, and sets some sensible defaults and data types.
 *
 * @see https://ws.ordermanagementsystems.com.au/omsordertaker/omsordertaker.asmx?op=CreateSalesOrder
 */
class SalesOrder {
  function __construct() {
    $this->RequestedShipDate = date(DATE_W3C);
    $this->RequestedDeliveryDate = date(DATE_W3C);
  }

  public $B2BCustomerName;
  public $B2BCustomerEmail;
  public $B2BCustomerPassword;

  public $EmailAddress;
  public $ExternalID;

  public $ClientRoleName;
  public $RefID = 0;

  // Bill to
  public $BillToCompany;
  public $BillToFName;
  public $BillToLName;
  public $BillToAddress1;
  public $BillToAddress2;
  public $BillToCity;
  public $BillToState;
  public $BillToZip;
  public $BillToCountry;
  public $BillToPhone;
  public $BillToResidential = TRUE;
  public $ShipToSameAsBillTo;

  // Shop to
  public $ShipToCompany;
  public $ShipToFName;
  public $ShipToLName;
  public $ShipToAddress1;
  public $ShipToAddress2;
  public $ShipToCity;
  public $ShipToState;
  public $ShipToZip;
  public $ShipToCountry;
  public $ShipToPhone;
  public $ShipToResidential = TRUE;

  // Other
  public $MerchantPONumber;
  public $MerchantOrderNumber;

  public $PricingDiscount = 0.00;
  public $CouponDiscount = 0.00;
  public $Handling = 0.00;
  public $FlatRateShipping = 0.00;
  public $Tax = 0.00;
  public $ShipFee = 0.00;
  public $ShipMethod;
  public $ShipAccount;
  public $POTerm = 'Net-10';
  public $ReferrerName;

  public $PaymentType;
  public $NameOnCard;
  public $CreditCardNumber;
  public $CreditCardExpMon = 0;
  public $CreditCardExpYr = 0;
  public $OptIn = FALSE;
  public $EmailCustomer = FALSE;

  public $Status = 'Processing';

  public $OrderLineItems;
  public $OrderPayments;
  public $OrderComments;

  public $Priority = 'Low';

  public $OrderDate;
  public $RequestedShipDate;
  public $RequestedDeliveryDate;

  public $BillToShipTo3PLID;
  public $Currency;
  public $Language;

  public $CheckForDuplicates = TRUE;
  public $HoursForDuplicateCheck = 48;
  public $ValidateOnlyMOrderID = TRUE;

  public $TransCode;
  public $AuthCode;
  public $Origin = 'Other'; // set to Drupal eventually

  public $LoginClientUserRoleID = 0;
  public $IPAddress;
  public $ShipToEmailAddress;
  public $NIPNumber;
  public $CheckOverrideLineItemTotal = FALSE;
  public $OverrideLineItemTotalAmount = 0;

  // EDI
  public $EDI_TransactionType;
  public $EDI_EmailClient = TRUE;
  public $EDI_FileName;
  public $EDI_NotBefore = 0;
  public $EDI_CancelAfter = 0;
  public $EDI_Description1;
  public $EDI_Description2;
  public $EDI_Description3;
  public $EDI_Description4;
  public $EDI_Description5;
}


/**
 * OrderLineItem
 *
 * This is a complex type defined by CreateSalesOrder representing the lineitems
 * for this order.
 *
 * This class lists all the properties as required by
 * the SOAP WSDL, and sets some sensible defaults and data types.
 *
 * @see https://ws.ordermanagementsystems.com.au/omsordertaker/omsordertaker.asmx?op=CreateSalesOrder
 */
class OrderLineItem {
  public $LineNumber;
  public $ItemName;
  public $QuantityOrdered;
  public $SKU;
  public $SKUWMS;
  public $UnitPrice;
  public $UnitPriceDiscount = 0;
  public $WarehouseName;
  public $OrderLineItem3PLID;
  public $ItemBarCode;
  public $Warehouse3PLID;
  public $SubInventory3PLID;
  public $BuyLoc;
  public $ShipLoc;
}
