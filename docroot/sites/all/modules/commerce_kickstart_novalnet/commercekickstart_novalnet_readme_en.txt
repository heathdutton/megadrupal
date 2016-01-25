/**                                                              
 * README INSTRUCTIONS                                          
 *                                                               
 * Direct Debit SEPA, Credit Card (3DSecure and non 3DSecure):    
 * Visa, MasterCard, Amex, JCB, CUP and Maestro, 		 
 * Prepayment, Invoice,                 			 
 * Online Transfer : eps, iDEAL and Instant Bank Transfer
 * Wallet system : PayPal
 *                                                               
 * These modules are programmed in high standard and supports	 
 * PCI DSS standard and the trusted shops standard used for 	 
 * real time processing of transactions through Novalnet	 
 *                                                               
 * Released under the GNU General Public License                 
 *                                                               
 * This free contribution made by request.                       
 * If you have found this script useful a small recommendation   
 * as well as a comment on merchant form would be greatly        
 * appreciated.                                                  
 *                                                               
 * Copyright (c) Novalnet AG   		                        
 *                                                               
 ******************************************************************
 *					   	   		 
 * SPECIFICATION DETAILS		   	   		 
 *								 
 * Created	     	 - Novalnet AG         	   		 
 *					   	   		 
 * Shop Version          - 1.x - 2.x   				 
 *					   	   		 
 * Novalnet Version      - 10.0.0			   	 
 *					   	   		 
 * Last Updated	         - 15-05-2015	   			 
 *					   	   		 
 * Compatibility         - Drupal 7.x - commercekickstart 1.x - 2.x  				 
 *					   	   		 
 * Stability	         - Stable		   	   	 
 *				  	   	   		 
 * Categories	         - Payment Gateways  	   		 
 *					   	   		 
 */

IMPORTANT:
----------
    The file "freewarelicenseagreement.txt" is part of this README file.


Installation Procedure:
-------------------------

    1. Make sure that you have curl installed in your system, if not please install curl, for installation help please visit "http://curl.haxx.se/docs/install.html".

    2. Unzip the novalnet includes and copy the folder "commerce_novalnet" and paste it in [root]/sites/all/modules/ directory.

    3. Now you can login in to shop admin panel and install our payment methods. Kindly follow the steps, explained in the "IG-drupal_v_7.x_commercekickstart_v_1.x-2.x_novalnet_v_10.0.0.pdf" file.

    4. Once all the settings completed, clear the cache via "Site settings-> Advanced settings-> Configuration-> Performance.

    Note:
    -----
    1. Do the following steps to display the transaction details in the order success page

        Login as Administrator navigate to Store settings-> Checkout settings

        Then click on the "configure" link of the "Completion message" checkout pane, Now "Completion message" checkout pane configuration form will open, in that form add "[commerce-order:novalnet_trxn_details]" token to the "Checkout completion message" where you need to display the Novalnet transaction details.

    2. Do the following steps to display the transaction details in the email.

        Login as Administrator navigate to Site settings-> Structure-> Message types

        Then click on the "edit" link of the "Commerce Order: order confirmation (Machine name: commerce_order_order_confirmation)" message type, Now "Edit Commerce Order: order confirmation" form will open, in that form add "[commerce-order:novalnet_trxn_details]" token at the end of the "View modes: Notify - Email body" field and click save message type.

Important Notice:
-----------------

	Kindly, contact sales@novalnet.de / tel. +49 (089) 923068320 to get the test data to process the payments.


CALLBACK SCRIPT:
----------------
    This is necessary for keeping your database/system actual and synchronize with the Novalnet transaction status.

    Your system will be notified through Novalnet system(asynchronous) about each transaction and its status.

    For example, if you use Novalnet "Invoice/Prepayment/PayPal" payment methods then on receival of the credit entry, your system will be notified through the Novalnet system and your system can automatically change the status of the order to the status which is configured(Ex: For Invoice & Prepayment status will be changed to Callback order status). Please use the "commerce_novalnet_callback_process.inc" provided in this payment package. Please follow the instructions in the "Callbackscript_testing_procedure.txt" file. You will find more details in the "commerce_novalnet_callback_process.inc" script itself.

Procedure to update callback script URL in Novalnet Administration area for callback script execution :
-----------------------------------------------------------------------------------------------------
    After logging into Novalnet Administration area, please choose your particular project navigate to "PROJECT" menu, then select appropriate "Project" and navigate to "Project Overview" tab and then update callback script url in "Vendor script URL" field.

    Ex: https://commercekickstart.novalnet.de/callback_novalnet

------------------------------------------------------------------------
AFFILIATE PROCESS: Follow the below necessary step to set up the process
------------------------------------------------------------------------
Set the shop website URL with the vendor id:

    For example: https://commercekickstart.novalnet.de/index.php?nn_aff_id=Vendor-ID


Please contact us on support@novalnet.de for further details
===========================================================
OUR CONTACT DETAILS / YOU CAN REACH US ON:

Tel.   : +49 (0)89 923 068 321
Web    : www.novalnet.de
E-mail : support@novalnet.de
==========================================================

