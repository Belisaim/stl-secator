// Options.h

#include <string.h>
#include <stdio.h>

////////////////////////////////////////
class Options
{
public:
  char *stl_file;
  float margin;    // in millimeters
  float format[2]; // in millimeters
  bool no_legend, no_preview;

  Options();
  void print_usage ();
  void init (char **cmdstr, int n);
  bool recognize (char *option);
};

// Global varioable
Options Opt;

//--------------------------
Options::Options()
{
  margin = 10;
  format[0] = 210; format[1] = 297; // A4
  no_legend = no_preview = false;
}

//-------------------------------------------------
void Options::init (char **cmdstr, int n)
{
  stl_file = cmdstr[n - 1];
  for (int i = 1; i < n - 1; i++)
    if (!recognize (cmdstr[i])) {
      fprintf (stderr, "Unknown option: %s\n\n", cmdstr[i]);
      exit (-10);
    }
}

//---------------------------------------
bool Options::recognize (char *option)
{
  if (strstr (option, "--no-preview")) { no_preview = true; return true; }
  if (strstr (option, "--no-legend" )) { no_legend  = true; return true; }
  return false;
}

//-----------------------------
void Options::print_usage ()
{
  puts ("\nUtility stl-secator is intended to make cross section of 3D object stored in ASCII STL file by the plane z = 0.");
  puts ("It is assumed to be used together with admesh utility.");
  puts ("Admesh cans convert binary STL to ASCII STL, move and turn 3D object in order to make cross section in any place of object.");
  puts ("Bash script geves possibility to automate this process.");
  puts ("The result of stl-secator is SVG files.");
  puts ("They are parts of one big poster (in general case) to be printed on small printer and pasted together.");
  puts ("Moreover, stl-secator outputs in stdout a data of cross section to display its by gnuplot for example.\n");
  puts ("Usage:");
  puts ("\tstl-secator [options] <ascii_stl_file>\n");
  puts ("Options:");
  puts ("\t--margin=n\tWidth of empty fields in edges on page for pasting together of a few pages.");
  puts ("\t\t\tn is float, millimeters.");
  puts ("\t\t\tBy default 10.");
  puts ("\t--format=x:y\tFormat of the paper.");
  puts ("\t\t\tx, y are float, millimeters.");
  puts ("\t\t\tBy default format A4: x = 210, y = 297.");
  puts ("\t--no-legend\tDo not print markers in SVG pictures.");
  puts ("\t\t\tBy default false.");
  puts ("\t--no-preview\tDo not create preview SVG file.");
  puts ("\t\t\tBy default false.");
  puts ("\nExample:");
  puts ("\t./stl-secator --margin=15 --format=297:420 boat.stl &>/dev/null\n\n");
}
