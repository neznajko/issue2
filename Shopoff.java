import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.Stack;
////////////////////////////////////////////////////////////////////////
class Offer {
  int   n; // number of <c, k> pairs
  int[] index; // basket index
  int[] count; // k
  int   offerPrice;
  public Offer(int n, int[] index, int[] count, int offerPrice) {
    this.n = n;
    this.index = Arrays.copyOf(index, n);
    this.count = Arrays.copyOf(count, n);
    this.offerPrice = offerPrice;
  }
  @Override
  public String toString() { /** */
    String s = String.format("%d ", n);
    for (int j = 0; j < n; ++j) {
      s += String.format("(%d, %d) ", index[j], count[j]);
    }
    return s + offerPrice;
  }
}
public class Shopoff { /** Bue c kakBo 6u7bo c4u? */
  static final int MAXN = 5; // maximum item types
  /* Item's code to index mapping */
  static HashMap<Integer, Integer> code = new HashMap<>();
  static int[] basket;
  static int[] itemPrice;
  static ArrayList<Offer> offers = new ArrayList<>();
  static Stack<Integer> stk = new Stack<>(); // history stack
  static int minTotal = Integer.MAX_VALUE;
  // Read and Load OFFER.TXT
  static void getOffer() throws IOException {
    BufferedReader buffer = null;
    try {
      buffer = new BufferedReader(new FileReader("OFFER.TXT"));
      String inputLine = buffer.readLine();
      int s = Integer.parseInt(inputLine); // Number of special offers.
      int[] index = new int [MAXN]; // buffers
      int[] count = new int [MAXN]; //
      loop: for (int i = 0; i < s; ++i) {
        inputLine = buffer.readLine();
        String[] ls = inputLine.trim().split(" +");
        int n = Integer.parseInt(ls[0]);
        // Discard offers that can't be applied to curent basket.
        if (basket.length < n) { continue; }           // first ck
        for (int j = 0; j < n; ++j) {                  //
          int c = Integer.parseInt(ls[1 + 2*j]);       //
          if (!code.containsKey(c)) { continue loop; } // second ck
          int item = code.get(c);                      //
          int k = Integer.parseInt(ls[2 + 2*j]);       //
          if (basket[item] < k) { continue loop; }     // third ck
          index[j] = item;
          count[j] = k;
        }
        int p = Integer.parseInt(ls[ls.length - 1]);
        offers.add(new Offer(n, index, count, p));
      }
    } finally {
      if (buffer != null) { buffer.close(); }
    }
  }
  /** He Me uHTepecyBa Ha Ko7Ko ka8eTa cTe, oT MeHe ceKu ntT
      uMaTe no egHa ncyBHs 3a 3gpaBe, kuBu u 3gpaBu */
  static void getInput() throws IOException {
    BufferedReader inputStream = null;
    try {
      inputStream = new BufferedReader(new FileReader("INPUT.TXT"));
      String line = inputStream.readLine();
      int b = Integer.parseInt(line);
      basket    = new int[b];
      itemPrice = new int[b];
      for (int j = 0; j < b; ++j) {
        line = inputStream.readLine();
        String[] buf = line.trim().split(" +");
        int c = Integer.parseInt(buf[0]);
        int k = Integer.parseInt(buf[1]);
        int p = Integer.parseInt(buf[2]);        
        code.put(c, j); /* <key(c), value(j)> */
        basket[j]    = k;
        itemPrice[j] = p;
      }
    }
    finally {
      if (inputStream != null) {
        inputStream.close();
      }}}
  /** check offer */
  static boolean ckoff(Offer ofr) {
    for (int i = 0; i < ofr.n; ++i) {
      if (basket[ofr.index[i]] < ofr.count[i]) {
        return false; }}
    return true; }
  /** apply offer */
  static void apply(Offer ofr) {
    for (int i = 0; i < ofr.n; ++i) {
      basket[ofr.index[i]] -= ofr.count[i];
    }}
  /** cancel offer */
  static void cancel(Offer ofr) {
    for (int i = 0; i < ofr.n; ++i) {
      basket[ofr.index[i]] += ofr.count[i];
    }}
  /** - KakBo e 3a Ba3e .., TaBa ro 3a6paBùx!,..?
      - 3a MeHe x5ro e .. */
  static int gettotal() {
    int total = 0;
    // basket
    for (int i = 0; i < basket.length; ++i) {
      if (0 < basket[i]) {
        total += basket[i] * itemPrice[i];
      }}
    // offers
    for (int i: stk) {
      total += offers.get(i).offerPrice;
    }
    return total;
  }
  /** - noHeke Mu ce npaBuTe Ha MHo apT, ga Bu Kaka 4e HsMa
        TaKuBa TtnaHopu KouTo ga cu nycKaT Te7eBu3opa c ugesTa
        ga r7egaT Ttnu peK7aMu o3By4eHu oT npoTuBeH r7aceF */
  static void backtrack() {
    for (int i = 0; i < offers.size(); ++i) {
      Offer ofr = offers.get(i);
      if (ckoff(ofr)) {
        apply(ofr);
        stk.push(i);
        backtrack();
      }}
    if(stk.empty()) { return; }
    int total = gettotal();
    if (total < minTotal) { minTotal = total; }
    cancel(offers.get(stk.pop()));
  }
  public static void main(String[] args) {
    System.out.println("Task: Shopping Offers");
    try {
      getInput();
      getOffer();
    } catch(IOException e) {
      e.printStackTrace();
    }
    offers.forEach((off) -> System.out.println(off));
    backtrack();
    System.out.println(minTotal);
  }}
////////////////////////////////////////////////////////////////////////
// log: ga ro gyxaTe r-H gkaHaBapoB
