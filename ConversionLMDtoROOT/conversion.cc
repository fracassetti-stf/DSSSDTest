// Program to convert Febex_TUM LMD files to ROOT

#include <iostream>
#include <fstream>
#include <string>
#include <inttypes.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "TObject.h"
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"

#include "TRandom.h"
#include "conversion.hh"

using namespace std;
using std::string;

const int MAX_FILES = 100;
const int BUF_SIZE=32768;
const int DBUF=65536;
//const int DBUF=16384;
const int EVENT_SIZE=40; //febex event size in bytes


bool source_in = false, source_out = false;


int i,j,k, frag_count=0;
int count_none=0;
int NUM_FILES;
uint32_t chval,ch;
size_t found;
time_t Start, End;  
double dif;
double width;

string lmd_name[MAX_FILES];
string base_name[MAX_FILES];
string root_name[MAX_FILES];
string current_root;
char name[150];
char hiname[50],hititle[50];

ifstream fin;
ifstream lmdin;

char filename[86]; // LMD filename read from the LMD file header
char date[24]; // start date and time

double si_offset,si_slope; // read calibration coefficients for Si from filenames file

FILE *      pFile;
long        lSize;
uint32_t *  buffer;
uint32_t *  frag_buffer;
uint32_t *  f;
uint32_t *  q ;
uint32_t *  q_gosiphead, * q_gosipstart ;
uint32_t *  q_startbuf, *q_startevt, *q_startsub ,*qcurrent, *q_endbuf, *q_startfrag;
uint32_t *  q_tmpstart, *q_tmpstop;
uint32_t *  f_startevt, *f_startsub , * f_gosipstart , * f_start, * f_second;


uint32_t result, filehead;
int num_buffers, oldbufnum=0;
int nofrag_flag =0;
uint64_t current_timestamp, 	oldfebextimestamp=0.;
uint64_t switch_time, 	oldswitch_time=0., diff_time;
uint32_t extralength; 
uint32_t offset;
uint32_t missing;
uint32_t value;
int32_t frag_length, endfrag_length,skipfrag;

gsibuffer mygsibuffer;
eventhead myeventhead;
sub_eventhead mysub_eventhead;
gosiphead mygosiphead;
febex myfebex;
event event_dssd[32];
fragment myfragment;


TH1F *esi_h[32];
TBranch *esi_b[32],*tsi_b[32] ;
TTree * sitree[32];
TRandom rr;


int main()
{

   time (&Start); // Determine file processing time


   //Init
   for (i=0;i<4;i++){mygsibuffer.l_free[i]=0.;}
   extralength = 0.;
   myfragment.dlen=0.;

   
   //Read LMD filenames from file filenams
   
   
   fin.open("filenames");
   if (fin.is_open()){
     printf("Opened file filenames \n");
   }
   else{
     cout << "Could not open file!\n";
   }

   fin>>NUM_FILES;
   fin.ignore();
   cout << "Number of files is "<< NUM_FILES<< "\n";

   i=0.;
   while (!fin.eof()){
     getline(fin,lmd_name[i]);
     cout<<lmd_name[i]<<" "<<i<<"\n";
     found=lmd_name[i].find('.');
     base_name[i]=lmd_name[i].substr(0,found);
     cout<<base_name[i]<<" "<<i<<"\n";
     root_name[i]=base_name[i].append(".root");
     i++;
   }
   
   fin.close();

 fin.open("calibration");
   if (fin.is_open()){
     //printf("Opened file calibration \n");
   }
   else{
     cout << "Could not open calibration file!\n";
   }

   fin>>si_slope;
   fin.ignore();
   fin>>si_offset;
   cout << "Si_slope "<< si_slope<< "\n";
   cout << "Si_offset "<< si_offset<< "\n";
   
   fin.close();


   
   cout <<"--------------------------------- \n";
   //open first LMD file for reading as a binary
   

   // ********************************** LOOP OVER LMD FILES ***********************************************************

   for (k=0;k<(int)NUM_FILES;k++){

   lmdin.open(lmd_name[k].c_str(),ios::binary);
   if (lmdin.is_open()){
     //printf("Opened binary file\n");
   }
   else{
     cout << "Could not open binary file!\n\n";
   }
   
   
   
   //********** Reading File header ****************************************************************

   lmdin.ignore(82); // skip 82 bytes - see description of the GSI file header struct s_filehe
   
   lmdin.read(filename,86);
   cout <<"Analyzing file  "<<filename<<"\n";
   
   lmdin.ignore(32); // skip 32 bytes
   
   lmdin.read(date,24);
   cout <<"Start date and time  "<<date<<"\n\n";
   
   lmdin.ignore(164); // skip 164 bytes
   lmdin.ignore(2400);// skip comment lines
   lmdin.ignore(29972);// skip extra bytes to make a 32K total header
   
   
   lmdin.close();

   
   //*********** Prepare root structures ***********************************************************
      
   for (ch=0;ch<32;ch++){
     sitree[ch] = new TTree ("sitree","sitree");
     esi_b[ch] = sitree[ch]->Branch("siheight",&event_dssd[ch].height,"siheight/S");
     tsi_b[ch] = sitree[ch]->Branch("sitime",&event_dssd[ch].timestamp,"sitime/l");
     sprintf(hiname,"h%d" , ch);
     sprintf(hititle,"Energy %d",ch);
     esi_h[ch] = new TH1F(hiname, hititle, 25000, 0,25000);
   }
   //esical_h = new TH1F("Esical", "Si energy calibrated", 8000, 0,8000);
   //width=esical_h->GetBinWidth(1);
   //cout << "WIDTH " << width <<"\n";
   
   //esical_h = new TH1F("Esical", "Si energy calibrated", 4000, 0,8000);
   
   
   current_root=root_name[k];
   strcpy(name,current_root.c_str());
   rootFile= new TFile(name,"RECREATE","Febex tree",1);
   if(!(rootFile->IsOpen())) printf("ERROR: Could not open root file!\n");
   
   
   
   //**********************************************************************************************
   num_buffers = 0;
   
   pFile = fopen ( lmd_name[k].c_str() , "rb" );
   if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
   
   // obtain file size:
   fseek (pFile , 0 , SEEK_END);
   lSize = ftell (pFile);
   rewind (pFile);
   cout << "File size is "<< lSize/1000. <<" kbytes\n";
   cout << "File size is "<< lSize/32768. <<" buffers\n";
   
   //**********************************************************************************************
   
   

  //allocate memory to contain two buffers
  buffer = (uint32_t*) malloc (sizeof(char)*DBUF);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  //allocate memory to contain fragment
  frag_buffer = (uint32_t*) malloc (sizeof(char)*DBUF);
  if (frag_buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  
  //Skip GSI file header
  filehead = fread (buffer,1,DBUF,pFile);
  fseek ( pFile , -BUF_SIZE , SEEK_CUR );


  //*************************************************************************************************
  //*************************************************************************************************
  //*************************************************************************************************
  //************************* Read buffers from the file ********************************************


  while (!feof(pFile)){
  //while (num_buffers < 5){
    
    offset = (num_buffers+1) * BUF_SIZE;  
    fseek ( pFile , offset , SEEK_SET );         //SEEK_SET:Beginning of file
    fread (buffer,1,DBUF,pFile);
    
    
    //read GSI buffer header

    num_buffers++;
    
    q = buffer;
    q_startbuf = q;                              // Pointer to start of GSI header

    mygsibuffer.dlen=*q++;  
    mygsibuffer.type=*q & 0x0000FFFF;
    mygsibuffer.subtype=(*q++ & 0xFFFF0000)>>16; 
    mygsibuffer.used=*q & 0x0000FFFF;

    q_endbuf = q_startbuf+((mygsibuffer.used+24)/2);// used: used words in the buffer after 24-word header
   
    mygsibuffer.fragments= (*q++ & 0xFFFF0000)>>16;
    mygsibuffer.end = mygsibuffer.fragments & 0x00FF;
    mygsibuffer.begin = (mygsibuffer.fragments & 0xFF00)>>8;       
    if (!mygsibuffer.end>0.){skipfrag=0.;}        //"NO Fragment END at begin of this buffer;  
    mygsibuffer.buf=*q++;
    //if (!((mygsibuffer.buf - oldbufnum) ==1)){cout <<"SOMETHING WRONG IN BUF NUM \n";}
    oldbufnum = mygsibuffer.buf; 
    mygsibuffer.evt=*q++;
    mygsibuffer.current=*q++;
    for (i=0;i<2;i++){
      mygsibuffer.l_time[i]=*q++;
    }   
    current_timestamp =  (((uint64_t)mygsibuffer.l_time[1] & 0x00000000FFFFFFFF)<<32)|mygsibuffer.l_time[0];
    //printf ("Current timestamp is in hex 0x%" PRIx64 " \n",current_timestamp);   
    for (i=0;i<4;i++){
      mygsibuffer.l_free[i]=*q++;  
    }
    //end of  GSI buffer header

    if ((num_buffers % 1000 )==0){printf("Reading buffer number %d \n", num_buffers);}
    

    
    for (q=q_startbuf+12+skipfrag;q-q_startbuf<((mygsibuffer.used+24)/2.);) // Add 12 longwords for GSI BUFFER header + fragment to be skipped
                                                                            // Till used buffer length + 12 longwords for GSI BUFFER header
                                                                            // Everything counted in long words, as q is of type uint32_t
      {
	
	
	if (!((q_endbuf - q)<(mygsibuffer.l_free[1]+4)/2)||nofrag_flag==1){ // checking if last event still fits the buffer. l_free[1] contains the event size  = data length +4 words 
	                                                                    // (q_endbuf-q) < datalength + 8 words event header = l_free[1] - 4 words + 8 words
                                                                            // nofrag_flag for events wrongly treated as fragment
	  nofrag_flag=0;

	  q_startevt = q;                                                   // Pointer to start of Event header 
	  
	  //************************ read event header ****************************************
	  
	  myeventhead.dlen =*q++;
	  myeventhead.type=*q & 0x0000FFFF;
	  myeventhead.subtype=(*q++ & 0xFFFF0000)>>16;
	  myeventhead.dummy=*q & 0x0000FFFF;
	  myeventhead.trigger=(*q++ & 0xFFFF0000)>>16;
	  myeventhead.count =*q++;
	  
	  //************************ read subevent header ****************************************
	  
	  mysub_eventhead.dlen =*q++;
	  mysub_eventhead.type=*q & 0x0000FFFF;
	  mysub_eventhead.subtype=(*q++ & 0xFFFF0000)>>16;
	  mysub_eventhead.procid=*q & 0x0000FFFF;
	  mysub_eventhead.fragments=(*q++ & 0xFFFF0000)>>16;
	  mysub_eventhead.subcrate = mysub_eventhead.fragments & 0x00FF;
	  mysub_eventhead.control = (mysub_eventhead.fragments & 0xFF00)>>8;

	  q_startsub = q;                                                   // Pointer to start of Subevent data (after subevent header)

	  //************************ read subevent ************************************************
	  
	  while (q<q_startsub+(myeventhead.dlen-10)/2) {                    // while over Data length = event length - 4 (definition) - 6 (subevent header) words

	    // Skip DMA alignment words
	    while((*q & 0xfff00000) == 0xadd00000){
	      ++q;     
	    }
	    
	    // GOSIP Header size
	    if((*q & 0xff) != 0x34)
	      {
		// Oopsy
		printf("[WARNING] Invalid GOSIP submemory header: 0x%08x\n", *q);
		break;
	      }
	    
	    q_gosiphead = q;
	    mygosiphead.channel = (*q++ >> 16) & 0xffff;
	    mygosiphead.bufsize = *q++;	

	    // Skip FEBEX special channel
	    if((mygosiphead.channel & 0xff00) == 0xff00)
	      {
		q += mygosiphead.bufsize/4; //Gosip header bufsize is in bytes
		continue;
	      }
	    
	    
	    for(q_gosipstart = q; q < q_gosipstart + mygosiphead.bufsize/4; ) //Gosip header bufsize is in bytes 
	      {
		// Check event header
		if(!(*q & 0xffff0000) == 0x115A0000)
		  {
		    // Oops... Something went wrong
		    printf("[WARNING] Invalid event header: 0x%08x\n", *q);
		    break;
		  }

		//Read data for this channel
		myfebex.size = *q & 0xffff; //size in bytes
		myfebex.magic = *q++ & 0xffff0000;
		myfebex.trigger = *q++;
		myfebex.time[0] = *q++;
		myfebex.time[1] = *q++;
		myfebex.timestamp = (uint64_t) myfebex.time[1]>>32 | (uint64_t) myfebex.time[0];
		
		if (myfebex.timestamp - oldfebextimestamp<0.){
		printf("THIS SHOULD NOT HAPPEN !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		}
		
		oldfebextimestamp = myfebex.timestamp;
		q++; //skip discriminator trace sample
		q++; //skip discriminator trace sample 
		myfebex.overflow = *q & 0xffffff;
		myfebex.self = *q++ & 0xff000000;
		myfebex.pileup = *q & 0xffff;
		myfebex.discard = *q++ & 0xffff0000;
		myfebex.height= *q++ & 0xffff;
		q++; //skip QPID

	
   

		for (ch=0;ch<16;ch++){
		    chval = (ch<<8)|0x00;
		    //cout << chval << "\n";
		    if (mygosiphead.channel ==chval){
		  
		      esi_h[ch]->Fill(myfebex.height);
		      //esical_h->Fill(si_slope*(myfebex.height+(rr.Rndm()-0.5)*width)+si_offset);
		      event_dssd[ch].height = myfebex.height;
		      event_dssd[ch].timestamp = myfebex.timestamp;
		      sitree[ch]->Fill();
		    }
		}	
		for (ch=16;ch<32;ch++){
		  chval = ((ch-16)<<8)|0x01;
		  //cout << chval << "\n";
		    if (mygosiphead.channel ==chval){
		  
		      esi_h[ch]->Fill(myfebex.height);
		      //esical_h->Fill(si_slope*(myfebex.height+(rr.Rndm()-0.5)*width)+si_offset);
		      event_dssd[ch].height = myfebex.height;
		      event_dssd[ch].timestamp = myfebex.timestamp;
		      sitree[ch]->Fill();
		    }
		}
		

		  
	      } // end for loop over gosip data
	    
	  }  // end loop over event

	} //end if still in buffer
      
            
      
	else {                                                                // It's a fragment!
	  //printf("It's a fragment \n");
	  frag_count++;
	  q_startfrag = q;
	  memset (frag_buffer, 0., DBUF);                                     // Init fragment buffer
	  memcpy (frag_buffer, q, 4*(q_endbuf-q));                            // Copy fragment to fragment buffer
	  
	  f_start=frag_buffer;
	  frag_length = (q_endbuf-q);
	  
	  for (i=0;i<frag_length;i++){                                        // Move pointer to end of first buffer
	    ++q;
	  }
	  
	  for (i=0;i<(int)(mygsibuffer.dlen-mygsibuffer.used)/2;i++){              // Skip unused part of the first buffer
	    ++q;
	  }
	  
	  for (i=0;i<12;i++){                                                 // Skip 24 words GSI buffer header for the second buffer in the double buffer
	    ++q;
	  }
	  
	  
	  q_tmpstart = q;                                                     // Pointer to start of fragment header
	  myfragment.dlen= *q++ & 0x0000FFFF;                                 // Read end-fragment header
	  myfragment.type=*q & 0x0000FFFF;
	  myfragment.subtype=(*q++ & 0xFFFF0000)>>16;
	  endfrag_length = (myfragment.dlen)/2;                               // In long words excl header
	  
	  memcpy ((frag_buffer+frag_length), q, 4*endfrag_length);            // Copy second part of fragment to fragment buffer
	                                                                      // frag_buffer now contains the whole event

	  
	  for (i=0;i<endfrag_length;i++){
	    q++;                                                              // Skip endfrag_length long words
	  } 

	  q_tmpstop = q;                                                      // Pointer to end of fragment data

	  skipfrag = (q_tmpstop-q_tmpstart);                                  // Number of long words to be skipped when reading the next bufer 
	                                                                      // exl GSI header, but including fragment header.
	


	  //********** READ DATA IN FRAG_BUFFER *********************

	  f=f_start;                                                          // Pointer to start of frag_buffer

	  //********** Reading event header *************************
	  myeventhead.dlen =*f++;
	  myeventhead.type=*f & 0x0000FFFF;
	  myeventhead.subtype=(*f++ & 0xFFFF0000)>>16;
	  myeventhead.dummy=*f & 0x0000FFFF;
	  myeventhead.trigger=(*f++ & 0xFFFF0000)>>16;
	  myeventhead.count =*f++;
      
	  //********** Reading sub_event header **********************
	  mysub_eventhead.dlen =*f++;
	  if ((myeventhead.dlen - mysub_eventhead.dlen)== 8){
	    //cout << "Oooops, this was not a fragment! \n";
	    q = q_startfrag;
	    nofrag_flag = 1;
	    continue;
	    
	  }
	  mysub_eventhead.type=*f & 0x0000FFFF;
	  mysub_eventhead.subtype=(*f++ & 0xFFFF0000)>>16;
	  mysub_eventhead.procid=*f & 0x0000FFFF;
	  mysub_eventhead.fragments=(*f++ & 0xFFFF0000)>>16;
	  mysub_eventhead.subcrate = mysub_eventhead.fragments & 0x00FF;
	  mysub_eventhead.control = (mysub_eventhead.fragments & 0xFF00)>>8;


	  f_startsub = f;                                                    // Pointer to start of subevent data (after subevent header)

	  while (f<f_startsub+(mygsibuffer.l_free[1]-10)/2){                 // While over Data length = event length - 4 (definition) - 6 (subevent header) words

	    // Skip DMA alignment words
	    while((*f & 0xfff00000) == 0xadd00000){
	      ++f;
	    }
	    
	    // GOSIP Header size
	    if((*f & 0xff) != 0x34)
	      {
		// Oopsy
		printf("[WARNING] Invalid GOSIP submemory header: 0x%08x\n", *q);
		break;
	      }
	    
	    mygosiphead.channel = (*f++ >> 16) & 0xffff;
	    mygosiphead.bufsize = *f++;	
	    
	    // Skip FEBEX special channel
	    if((mygosiphead.channel & 0xff00) == 0xff00)
	      {
		f += mygosiphead.bufsize/4;                                     // Gosip header bufsize is in Bytes
		continue;
	      }
	    
	    
	    for(f_gosipstart = f; f < (f_gosipstart + mygosiphead.bufsize/4); ) // Gosip header bufsize is in bytes 
	      {
		// Check event header
		if(!(*f & 0xffff0000) == 0x115A0000)
		  {
		    // Oops... Something went wrong
		    fprintf(stderr, "[WARNING] Invalid event header: 0x%08x\n", *q);
		    break;
		  }

		//Read data for this channel
		myfebex.size = *f & 0xffff;                                     // Size in bytes
		myfebex.magic = *f++ & 0xffff0000;
		myfebex.trigger = *f++;
		myfebex.time[0] = *f++;
		myfebex.time[1] = *f++;
		myfebex.timestamp = (uint64_t) myfebex.time[1]>>32 | (uint64_t) myfebex.time[0];
		if (myfebex.timestamp - oldfebextimestamp<0.){
		printf("THIS SHOULD NOT HAPPEN !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		}
		oldfebextimestamp = myfebex.timestamp;
		f++; //skip discriminator trace sample
		f++; //skip discriminator trace sample 
		myfebex.overflow = *f & 0xffffff;
		myfebex.self = *f++ & 0xff000000;
		myfebex.pileup = *f & 0xffff;
		myfebex.discard = *f++ & 0xffff0000;
		myfebex.height= *f++ & 0xffff;
		f++; //skip QPID

		for (ch=0;ch<16;ch++){
		    chval = (ch<<8)|0x00;
		    //cout << chval << "\n";
		    if (mygosiphead.channel ==chval){
		  
		      esi_h[ch]->Fill(myfebex.height);
		      //esical_h->Fill(si_slope*(myfebex.height+(rr.Rndm()-0.5)*width)+si_offset);
		      event_dssd[ch].height = myfebex.height;
		      event_dssd[ch].timestamp = myfebex.timestamp;
		      sitree[ch]->Fill();
		    }
		}

		
		for (ch=16;ch<32;ch++){
		  chval = ((ch-16)<<8)|0x01;
		  //cout << chval << "\n";
		  if (mygosiphead.channel ==chval){
		  
		      esi_h[ch]->Fill(myfebex.height);
		      //esical_h->Fill(si_slope*(myfebex.height+(rr.Rndm()-0.5)*width)+si_offset);
		      event_dssd[ch].height = myfebex.height;
		      event_dssd[ch].timestamp = myfebex.timestamp;
		      sitree[ch]->Fill();
		    }
		}
		


		
	      }  // end for loop over gosip data fragment
	    
	  }  // end while in event
	  
	}  // END ELSE
      } // end while in buffer
    
  
  } 
  
  cout<< "Reached end of file \n\n";
  cout<< frag_count << " fragments \n";
  cout << "events before in/out flags ... " << count_none << "\n";
  
  
  
  for (ch = 0;ch<32;ch++){
  esi_h[ch]->Write();
  }
  sitree[0]->Write();

  rootFile->Write();
  rootFile->Close();
  
  
  // terminate  

  free (frag_buffer);
  free (buffer);
  fclose (pFile);
  gDirectory->GetList()->Delete();   // Delete histograms from the memory


  time (&End);
  dif = difftime(End,Start);
  printf("\n Processed in : %.6f seconds \n",dif);
  
   } // END for loop over LMD files
   return 0;
   
}










//DRAW command
//getree->Draw("geheight>>h1(2000,0,2000)","getime>0.")
/*if (myfebex.magic == 0x2BAD){
  
  cout<< "2 bad ++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  cout<< "2 bad ++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  cout<< "2 bad ++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  cout<< "2 bad ++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  
  q++; //
  q++; //
  q++;q++;q++;q++;q++;q++;
  continue;
  }
*/
/*	  if ((myeventhead.dlen - mysub_eventhead.dlen)== 8){
	    //cout<<"CORRECT ! \n";
	  }
	  else {
	    //cout<<"NOT CORRECT ! \n";
	  }
*/
