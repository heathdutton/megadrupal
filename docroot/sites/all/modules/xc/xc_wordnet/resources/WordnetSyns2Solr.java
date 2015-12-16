import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.FileWriter;
import java.io.PrintStream;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
//import java.util.Set;
import java.util.TreeMap;
//import java.util.TreeSet;

/**
 * This class is a rewriten version of Syns2Syms class available at https://gist.github.com/562776,
 * and authored by Christopher Bradford (https://github.com/bradfordcp).
 * This version was created by Király Péter for eXtensible Catalog Organization
 *
 * It converts the prolog files wn_s.pl, wn_sk.pl, and wn_g.pl, from the
 * <a href="http://wordnetcode.princeton.edu/3.0/WNprolog-3.0.tar.gz">WordNet prolog
 * download</a> into an XML file which consumable with Apache Solr.
 *
 * This has been tested with WordNet 3.0.
 *
 * The structure of the resulted Solr documents is the following:
 *
 * <doc>
 *   <field name="id">100001740</field>
 *   <field name="gloss_t">that which is perceived or known or inferred to have its own distinct existence (living or nonliving)</field>
 *   <field name="lexfile_s">03</field>
 *   <field name="lexdict_s">noun.Tops</field>
 *   <field name="word_t">entity</field>
 *   <field name="word_s">entity</field>
 * </doc>
 *
 * The fields:
 * - id: the synset identifier
 * - gloss_t: the glossary (the meaning of the word group)
 * - lexfile_s: the number of lexfile
 * - lexdict: the machine name of the lexfile
 * - word_t: the word(s) belonging to this group indexable as text
 * - word_s: the word(s) belonging to this group indexable as phrase
 *
 * Usage:
 *   1) download WordNet prolog from http://wordnetcode.princeton.edu/3.0/WNprolog-3.0.tar.gz
 *   2) extract it to a convenient place
 *   3) convert prolog to XML with:
 *       java WordnetSyns2Solr <prolog directory> <output file>
 *   4) update it to Solr
 *       curl <Solr URL> --data-binary @<output file> -H "Content-type: application/xml"
 *
 * @see <a href="http://wordnet.princeton.edu/">WordNet home page</a>
 * @see <a href="http://wordnet.princeton.edu/wordnet/documentation/">WordNet documentation page, including the Prolog files documentation</a>
 */
public class WordnetSyns2Solr {
	/**
   *
   */
	private static final PrintStream o = System.out;

	/**
   *
   */
	private static final PrintStream err = System.err;
	
	private static Map<String, String> lexDict = new TreeMap<String, String>();
	
	static {
		lexDict.put("00", "adj.all");
		lexDict.put("01", "adj.pert");
		lexDict.put("02", "adv.all");
		lexDict.put("03", "noun.Tops");
		lexDict.put("04", "noun.act");
		lexDict.put("05", "noun.animal");
		lexDict.put("06", "noun.artifact");
		lexDict.put("07", "noun.attribute");
		lexDict.put("08", "noun.body");
		lexDict.put("09", "noun.cognition");
		lexDict.put("10", "noun.communication");
		lexDict.put("11", "noun.event");
		lexDict.put("12", "noun.feeling");
		lexDict.put("13", "noun.food");
		lexDict.put("14", "noun.group");
		lexDict.put("15", "noun.location");
		lexDict.put("16", "noun.motive");
		lexDict.put("17", "noun.object");
		lexDict.put("18", "noun.person");
		lexDict.put("19", "noun.phenomenon");
		lexDict.put("20", "noun.plant");
		lexDict.put("21", "noun.possession");
		lexDict.put("22", "noun.process");
		lexDict.put("23", "noun.quantity");
		lexDict.put("24", "noun.relation");
		lexDict.put("25", "noun.shape");
		lexDict.put("26", "noun.state");
		lexDict.put("27", "noun.substance");
		lexDict.put("28", "noun.time");
		lexDict.put("29", "verb.body");
		lexDict.put("30", "verb.change");
		lexDict.put("31", "verb.cognition");
		lexDict.put("32", "verb.communication");
		lexDict.put("33", "verb.competition");
		lexDict.put("34", "verb.consumption");
		lexDict.put("35", "verb.contact");
		lexDict.put("36", "verb.creation");
		lexDict.put("37", "verb.emotion");
		lexDict.put("38", "verb.motion");
		lexDict.put("39", "verb.perception");
		lexDict.put("40", "verb.possession");
		lexDict.put("41", "verb.social");
		lexDict.put("42", "verb.stative");
		lexDict.put("43", "verb.weather");
		lexDict.put("44", "adj.ppl");
	}

	/**
	 * Takes arg of prolog file name and output file
	 */
	public static void main(String[] args) throws Throwable {
		// get command line arguments
		String directory = null; // name of file "wn_s.pl"
		String prologFilename = "wn_s.pl"; // name of file "wn_s.pl"
		String glossFilename = "wn_g.pl";
		String keyFilename = "wn_sk.pl";
		String outputFilename = null;
		if (args.length == 2) {
			directory = args[0];
			outputFilename = args[1];
		} else {
			usage();
			System.exit(1);
		}

		// ensure that the prolog file is readable
		if (!(new File(directory + "/" + prologFilename)).canRead()) {
			err.println("Error: cannot read Prolog file: " + directory + "/" + prologFilename);
			System.exit(1);
		}
		// ensure that the output file is writeable
		if (!(new File(outputFilename)).canWrite()) {
			if (!(new File(outputFilename)).createNewFile()) {
				err.println("Error: cannot write output file: " + outputFilename);
				System.exit(1);
			}
		}

		o.println("Opening Prolog file " + prologFilename);
		final FileInputStream fis = new FileInputStream(directory + "/" + prologFilename);
		BufferedReader br = new BufferedReader(new InputStreamReader(fis));
		String line;

		// maps a word to all the "groups" it's in
		final Map<String, List<String>> word2Nums = new TreeMap<String, List<String>>();
		// maps a group to all the words in it
		final Map<String, List<String>> num2Words = new TreeMap<String, List<String>>();
		// number of rejected words
		int ndecent = 0;

		// status output
		int mod = 1;
		int row = 1;

		// parse prolog file
		o.println("[1/2] Parsing " + prologFilename);
		while ((line = br.readLine()) != null) {
			// occasional progress
			if ((++row) % mod == 0) { // periodically print out line we read in
				mod *= 2;
				o.println("\t" + row + " " + line + " " + word2Nums.size()
						+ " " + num2Words.size() + " ndecent=" + ndecent);
			}

			// syntax check
			if (!line.startsWith("s(")) {
				err.println("OUCH: " + line);
				System.exit(1);
			}

			// parse line
			// example: s(100001740,1,'entity',n,1,11).
			// s(synset_id,w_num,'word',ss_type,sense_number,tag_count).
			line = line.substring(2); // 100001740,1,'entity',n,1,11).
			int comma = line.indexOf(',');
			String num = line.substring(0, comma); // num = 100001740
			int q1 = line.indexOf('\'');
			line = line.substring(q1 + 1);
			int q2 = line.lastIndexOf('\'');
			String word = line.substring(0, q2).toLowerCase().replace("''", "'"); // word = entity

			// 1/2: word2Nums map
			// append to entry or add new one
			List<String> lis = word2Nums.get(word);
			if (lis == null) {
				lis = new LinkedList<String>();
				lis.add(num);
				word2Nums.put(word, lis);
			} else {
				lis.add(num);
			}

			// 2/2: num2Words map
			lis = num2Words.get(num);
			if (lis == null) {
				lis = new LinkedList<String>();
				lis.add(word);
				num2Words.put(num, lis);
			} else {
				lis.add(word);
			}
		}

		// close the streams
		fis.close();
		br.close();
		
		o.println("[1/2] Parsing " + glossFilename);
		final Map<String, String> num2Gloss = new TreeMap<String, String>();
		br = new BufferedReader(
				new InputStreamReader(
						new FileInputStream(directory + "/" + glossFilename)));
		while ((line = br.readLine()) != null) {
			if (!line.startsWith("g(")) {
				err.println("OUCH: " + line);
				System.exit(1);
			}

			// parse line
			// g(102084071,'a member of the genus Canis (probably descended from the common wolf) that has been domesticated by man since prehistoric times; occurs in many breeds; "the dog barked all night"').
			// s(synset_id,w_num,'word',ss_type,sense_number,tag_count).
			String num = line.substring(2, 11); // num = 100001740
			int q1 = line.indexOf("'") + 1;
			int q2;
			if (line.indexOf(';') > -1) {
				q2 = line.indexOf(';');
			}
			else {
				q2 = line.lastIndexOf("'");
			}
			String gloss = line.substring(q1, q2)
								.replace("''", "'")
								.replace("&", "&amp;")
								.replace("<", "&lt;")
								.replace(">", "&gt;");
			num2Gloss.put(num, gloss);
		}
		br.close();

		o.println("[1/2] Parsing " + keyFilename);
		final Map<String, String> num2Lex = new TreeMap<String, String>();
		br = new BufferedReader(
				new InputStreamReader(
						new FileInputStream(directory + "/" + keyFilename)));
		while ((line = br.readLine()) != null) {
			if (!line.startsWith("sk(")) {
				err.println("OUCH: " + line);
				System.exit(1);
			}

			// parse line
			// sk(102084071,1,'dog%1:05:00::').
			String num = line.substring(3, 12); // num = 100001740
			if (!num2Lex.containsKey(num)) {
				int q1 = line.indexOf('%') + 1;
				int q2 = line.lastIndexOf("'");
				String[] lex_sense = line.substring(q1, q2).split(":");
				num2Lex.put(num, lex_sense[1]);
			}
		}
		br.close();


		// create the index
		o.println("[2/2] Building index to store synonyms, "
				+ " map sizes are " + word2Nums.size() + " and "
				+ num2Words.size());
		index(outputFilename, word2Nums, num2Words, num2Gloss, num2Lex);
	}

	/**
	 * Forms a static text file based on the 2 maps.
	 *
	 * @param outputFileName
	 *            the file where the synonyms should be created
	 * @param word2Nums
	 * @param num2Words
	 */
	private static void index(String outputFileName,
			Map<String, List<String>> word2Nums,
			Map<String, List<String>> num2Words,
			Map<String, String> num2Gloss,
			Map<String, String> num2Lex) throws Throwable {

		o.println("Opening output file " + outputFileName);
		FileWriter output_writer = new FileWriter(outputFileName);

		output_writer.write("<?xml version=\"1.0\"?>\n");
		output_writer.write("<add>\n");
		try {
			Iterator<String> i1 = num2Words.keySet().iterator();
			while (i1.hasNext()) { // for each word
				String synset_id = i1.next();
				StringBuilder builder = new StringBuilder();

				builder.append("<doc>\n");
				builder.append("\t<field name=\"id\">" + synset_id + "</field>\n");
				builder.append("\t<field name=\"gloss_t\">" + num2Gloss.get(synset_id) + "</field>\n");
				builder.append("\t<field name=\"lexfile_s\">" + num2Lex.get(synset_id) + "</field>\n");
				builder.append("\t<field name=\"lexdict_s\">" + lexDict.get(num2Lex.get(synset_id)) + "</field>\n");
				
				// words
				List<String> words = num2Words.get(synset_id);
				Iterator<String> i2 = words.iterator();
				while (i2.hasNext()) { // for each key#
					String word = i2.next();
					builder.append("\t<field name=\"word_t\">" + word + "</field>\n");
					builder.append("\t<field name=\"word_s\">" + word + "</field>\n");
				}
				builder.append("</doc>\n");
				
				output_writer.write(builder.toString());
			}
		} finally {
			output_writer.write("</add>\n");
			output_writer.close();
		}
	}

	/**
	 * Usage message to aide nooblets
	 */
	private static void usage() {
		o.println("\n\n"
			+ "Generates the appropriate synonyms database in a format consumable by Apache Solr\n"
			+ "Usage: java WordnetSyns2Solr <prolog directory> <output file>\n"
			+ "Example: java WordnetSyns2Solr ../prolog synonyms.xml\n");
	}
}
