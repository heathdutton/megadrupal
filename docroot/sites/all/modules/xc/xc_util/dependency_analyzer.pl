# dependency_analyzer.pl
# Trace the functions a module calls which are not defined inside the modules,
# and not PHP, nor Drupal functions.
# usage: copy this file inside a module, and run as
# > perl -w dependency_analyzer.pl

use constant LN => "\n";
use strict;


my @files = <*.*>;
push(@files, <*/*.*>);

my %php_functions = ('array', 1, 'array_diff', 1, 'array_filter', 1, 'array_keys', 1, 'array_map', 1,
  'array_merge', 1, 'array_search', 1, 'array_unique', 1, 'base64_decode', 1, 'base64_encode', 1,
  'count', 1, 'create_function', 1, 'define', 1, 'empty', 1, 'explode', 1, 'file_get_contents', 1,
  'file_put_contents', 1, 'file_scan_directory', 1, 'foreach', 1, 'get_object_vars', 1, 'gettype', 1,
  'htmlentities', 1, 'if', 1, 'implode', 1, 'in_array', 1, 'include_once', 1, 'intval', 1, 'is_array', 1,
  'is_null', 1, 'is_object', 1, 'is_string', 1, 'isset', 1, 'join', 1, 'list', 1, 'max', 1,
  'microtime', 1, 'nl2br', 1, 'preg_match', 1, 'preg_match_all', 1, 'preg_replace', 1,
  'preg_replace_callback', 1, 'printf', 1, 'rawurlencode', 1, 'split', 1, 'sprintf', 1,
  'stdClass', 1, 'str_replace', 1, 'stream_context_create', 1, 'strstr', 1, 'substr', 1,
  'usleep', 1, 'parse_url', 1, 'trim', 1, 'unset', 1, 'urldecode', 1, 'urlencode', 1,
  'var_export', 1, 'while', 1, 'function_exists', 1);
my %drupal_functions = ('db_drop_table', 1, 'db_fetch_object', 1, 'db_query', 1, 'db_rewrite_sql', 1,
  'db_table_exists', 1, 'drupal_add_css', 1, 'drupal_add_js', 1, 'drupal_add_tabledrag', 1,
  'drupal_attributes', 1, 'drupal_get_destination', 1, 'drupal_get_form', 1, 'drupal_get_path', 1,
  'drupal_get_schema_unprocessed', 1, 'drupal_goto', 1, 'drupal_install_schema', 1, 'drupal_render', 1,
  'drupal_set_header', 1, 'drupal_set_message', 1, 'drupal_set_title', 1, 'drupal_uninstall_schema', 1,
  'drupal_write_record', 1, 'element_children', 1, 'form_set_error', 1, 'form_set_value', 1,
  'format_plural', 1, 'hook_simpletest', 1, 'l', 1, 'module_exists', 1, 'pager_query', 1,
  'tablesort_header', 1, 'tablesort_init', 1, 't', 1, 'theme', 1, 'url', 1, 'user_access', 1,
  'variable_del', 1, 'variable_get', 1, 'variable_set', 1);

my %defined_functions = ();
my %called_functions  = ();

my $code;
foreach my $file (@files) {
	if ($file !~ /\.(module|php|inc|install)$/) {
		next;
	}
	open(PHP, $file);
	$code = do { local $/; <PHP> };
	close PHP;
	
	# remove comments
	$code =~ s/\/\*\*\s*\n( \*[^\n]+\n)+ \*\/\s*\n//g;
	# remove commented blocks
	$code =~ s/\/\*.*?\*\///g;
	# remove commented lines
	$code =~ s/\/\/.*?\n/\n/g;
	
	if ($file =~ /\.install/) {
		$code =~ s/(function )(xc_\w+_update_\d+\(\)\s\{.*?function )+(xc_\w+_update_)/$1$3/s;
	}

	my @functions = ($code =~ /function (\w+)\(/g);
	for (my $i = 0; $i<=$#functions; $i++) {
		if (exists $defined_functions{$functions[$i]}) {
			print STDERR $functions[$i], ' exists', LN;
		}
		else {
			$defined_functions{$functions[$i]} = 1;
		}
	}

	@functions = ($code =~ /(?!function)[\n \!\(=](\w+)\(/g); # \W := \n, ' ', !, (, >, :, =, $
	for (my $i = 0; $i<=$#functions; $i++) {
		if (exists $php_functions{$functions[$i]} || exists $drupal_functions{$functions[$i]}) {
			next;
		}
		unless(exists $called_functions{$functions[$i]}) {
			$called_functions{$functions[$i]} = 1;
		}
	}
}

my $fn;
print STDERR 'Functions declared outside of the module:', LN;
print STDERR '==========================', LN;
foreach $fn (sort {$a cmp $b} keys %called_functions) {
	unless(exists $defined_functions{$fn}) {
		print STDERR $fn, LN;
	}
}