/* Word Length Counter
*
* Modified by Nick Graham
* This code is modified from the word count example found at
* http://hadoop.apache.org/docs/current/hadoop-mapreduce-client/hadoop-mapreduce-client-core/MapReduceTutorial.html
* Hadoop FileIO based off of https://wiki.apache.org/hadoop/HadoopDfsReadWriteExample
*/

import java.io.IOException;
import java.io.*;
import java.util.*;
import java.util.StringTokenizer;
import java.util.regex.Pattern;
import java.util.regex.Matcher;
import java.util.ArrayList;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.*;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

public class WordCount extends Configured implements Tool {
    Configuration myConf;
    public static class TokenizerMapper
    extends Mapper<Object, Text, Text, IntWritable>{

        private final static IntWritable one = new IntWritable(1);
        private Text word = new Text();

        public void map(Object key, Text value, Context context
        ) throws IOException, InterruptedException {
            Pattern p = Pattern.compile("[a-zA-z]");
            StringTokenizer itr = new StringTokenizer(value.toString());
            while (itr.hasMoreTokens()) {
                word.set(itr.nextToken());
                String wordVal = word.toString();
                int numChars = 0;
                for (int i = 0; i < wordVal.length(); i++) {
                    Matcher m = p.matcher(Character.toString(wordVal.charAt(i)));
                    if (m.matches()) {
                        numChars++;
                    }
                }
                if (numChars != 0) {
                    word.set(Integer.toString(numChars));
                    context.write(word, one);
                }
            }
        }
    }

    public static class IntSumReducer
    extends Reducer<Text,IntWritable,Text,IntWritable> {
        private IntWritable result = new IntWritable();

        ArrayList<Text> keys = new ArrayList<Text>();
        ArrayList<Integer> vals = new ArrayList<Integer>();

        public void reduce(Text key, Iterable<IntWritable> values,
        Context context
        ) throws IOException, InterruptedException {
            int sum = 0;
            for (IntWritable val : values) {
                sum += val.get();
            }
            result.set(sum);
            keys.add(key);
            vals.add(sum);
            context.write(key, result);
        }

        // public void cleanup(Context context) throws IOException, InterruptedException {
        //     for (int i = 0; i < keys.size(); i++) {
        //         result.set(vals.get(i));
        //
        //         context.write(keys.get(i), result);
        //     }
        // }

    }
    /*
    public Configuration getConf() {
        return myConf;
    }
    public void setConf(Configuration c) {
        myConf = c;
    }
    */
    public int run(String[] args) throws Exception {
        long start = System.currentTimeMillis();
        Configuration conf = getConf();
        Job job = Job.getInstance(conf, "word count");
        job.setJarByClass(WordCount.class);
        job.setMapperClass(TokenizerMapper.class);
        job.setCombinerClass(IntSumReducer.class);
        job.setReducerClass(IntSumReducer.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(IntWritable.class);
        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]+"tmp"));
        int res = job.waitForCompletion(true) ? 0 : 1;
        if (res != 0) {
            return (res);
        }

        Configuration formatConfig = new Configuration();
        FileSystem fs = FileSystem.get(conf);
        FileStatus[] fileStatus = fs.listStatus(new Path(args[1]+"tmp"));
        Path infile = new Path(args[1]+"tmp");;
        for (FileStatus fileStat : fileStatus) {
            infile = (fileStat.getPath());
        }
        Path outfile = new Path(args[1]);
        if (!fs.exists(infile)) {
            System.err.println("Input file not found");
            return (2);
        }
        if (!fs.isFile(infile)) {
            System.err.println("Input should be a file");
            return (2);
        }
        if (fs.exists(outfile)) {
            System.err.println("Output already exists");
            return (2);
        }
        FSDataInputStream in = fs.open(infile);
        BufferedReader d = new BufferedReader(new InputStreamReader(in));
        FSDataOutputStream out = fs.create(outfile);
        String line = null;
        HashMap<Integer, Integer> key_val = new HashMap<Integer, Integer>();
        Integer max_val = Integer.MIN_VALUE;
        Integer min_index = Integer.MAX_VALUE;
        Integer max_index = Integer.MIN_VALUE;
        while ((line = d.readLine()) != null) {
            String[] line_pair = line.split("\\s+");
            if (line_pair.length == 2) {
                key_val.put(new Integer(Integer.parseInt(line_pair[0])), Integer.parseInt(line_pair[1]));
                if (Integer.parseInt(line_pair[1]) > max_val) {
                    max_val = Integer.parseInt(line_pair[1]);
                }
                if (Integer.parseInt(line_pair[0]) < min_index) {
                    min_index = Integer.parseInt(line_pair[0]);
                }
                if (Integer.parseInt(line_pair[0]) > max_index) {
                    max_index = Integer.parseInt(line_pair[0]);
                }
            }
        }

        double width = 65;
        double ratio = width/max_val;
        for (int i = min_index; i <= max_index; i++) {
            if (key_val.containsKey(i)) {
                String tmp = String.format("%2s: ", Integer.toString(i));
                //System.err.printf("%2s: ", Integer.toString(i));
                for (double j = 0; j <  ratio * key_val.get(i); j++) {
                    tmp += "*";
                }
                for (double j =  ratio * key_val.get(i); j <= width; j++) {
                    tmp += " ";
                }
                tmp += String.format(" %-6s\n", Integer.toString(key_val.get(i)));
                out.writeChars(tmp);
                System.err.print(tmp);
            }
        }
        long end = System.currentTimeMillis();
        long duration = end - start;
        System.err.print(Long.toString(duration));
        out.writeChars(Long.toString(duration));
        //System.err.println(max_val);
        in.close();
        out.close();
        return res;
        //System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
    public static void main(String[] args) throws Exception {
        // Let ToolRunner handle generic command-line options
        int res = ToolRunner.run(new Configuration(), new WordCount(), args);

        System.exit(res);
    }
}
