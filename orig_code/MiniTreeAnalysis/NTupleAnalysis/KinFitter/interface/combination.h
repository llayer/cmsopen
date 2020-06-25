
1	//=======================================================
2	// combination.h
3	// Description : Template class to find combinations
4	//=======================================================
5	// Copyright 2003 - 2006 Wong Shao Voon
6	// No warranty, implied or expressed, is included.
7	// Author is not liable for any type of loss through 
8	// the use of this source code. Use it at your own risk!
9	//=======================================================
10	
11	
12	#ifndef __COMBINATION_H__
13	#define __COMBINATION_H__
14	
15	
16	namespace stdcomb
17	{
18	
19	// Non recursive template function
20	template <class BidIt>
21	
22	inline bool next_combination(BidIt n_begin, BidIt n_end,
23	BidIt r_begin, BidIt r_end)
24	{
25	  
26	  bool boolmarked=false;
27	  BidIt r_marked;
28	  
29	  BidIt n_it1=n_end;
30	  --n_it1;
31	  
32	  
33	  BidIt tmp_r_end=r_end;
34	  --tmp_r_end;
35	  
36	  for(BidIt r_it1=tmp_r_end; r_it1!=r_begin || r_it1==r_begin; --r_it1,--n_it1)
37	  {
38	    if(*r_it1==*n_it1 )
39	    {
40	      if(r_it1!=r_begin) //to ensure not at the start of r sequence
41	      {
42	        boolmarked=true;
43	        r_marked=(--r_it1);
44	        ++r_it1;//add it back again 
45	        continue;
46	      }
47	      else // it means it is at the start the sequence, so return false
48	        return false;      
49	    }
50	    else //if(*r_it1!=*n_it1 )
51	    {
52	      //marked code
53	      if(boolmarked==true)
54	      {
55	        //for loop to find which marked is in the first sequence
56	        BidIt n_marked;//mark in first sequence
57	        for (BidIt n_it2=n_begin;n_it2!=n_end;++n_it2)
58	          if(*r_marked==*n_it2) {n_marked=n_it2;break;}
59	      
60	    
61	        BidIt n_it3=++n_marked;    
62	        for  (BidIt r_it2=r_marked;r_it2!=r_end;++r_it2,++n_it3)
63	        {
64	          *r_it2=*n_it3;
65	        }
66	        return true;
67	      }
68	      for(BidIt n_it4=n_begin; n_it4!=n_end; ++n_it4)
69	        if(*r_it1==*n_it4)
70	        {
71	          *r_it1=*(++n_it4);
72	          return true;           
73	        }
74	    }
75	  }  
76	
77	  return true;//will never reach here    
78	}
79	
80	// Non recursive template function with Pred
81	template <class BidIt, class Prediate>
82	
83	inline bool next_combination(
84	        BidIt n_begin, 
85	        BidIt n_end,
86	        BidIt r_begin, 
87	        BidIt r_end,
88	        Prediate Equal)
89	{
90	  
91	  bool boolmarked=false;
92	  BidIt r_marked;
93	  
94	  BidIt n_it1=n_end;
95	  --n_it1;
96	  
97	  
98	  BidIt tmp_r_end=r_end;
99	  --tmp_r_end;
100	  
101	  for(BidIt r_it1=tmp_r_end; r_it1!=r_begin || r_it1==r_begin; --r_it1,--n_it1)
102	  {
103	    if( Equal( *r_it1, *n_it1) )
104	    {
105	      if(r_it1!=r_begin) //to ensure not at the start of r sequence
106	      {
107	        boolmarked=true;
108	        r_marked=(--r_it1);
109	        ++r_it1;//add it back again 
110	        continue;
111	      }
112	      else // it means it is at the start the sequence, so return false
113	        return false;      
114	    }
115	    else //if(*r_it1!=*n_it1 )
116	    {
117	      //marked code
118	      if(boolmarked==true)
119	      {
120	        //for loop to find which marked is in the first sequence
121	        BidIt n_marked;//mark in first sequence
122	        for (BidIt n_it2=n_begin;n_it2!=n_end;++n_it2)
123	          if( Equal( *r_marked, *n_it2) ) {n_marked=n_it2;break;}
124	      
125	    
126	        BidIt n_it3=++n_marked;    
127	        for  (BidIt r_it2=r_marked;r_it2!=r_end;++r_it2,++n_it3)
128	        {
129	          *r_it2=*n_it3;
130	        }
131	        return true;
132	      }
133	      for(BidIt n_it4=n_begin; n_it4!=n_end; ++n_it4)
134	        if( Equal(*r_it1, *n_it4) )
135	        {
136	          *r_it1=*(++n_it4);
137	          return true;           
138	        }
139	    }
140	  }  
141	
142	  return true;//will never reach here    
143	}
144	
145	
146	// Non recursive template function
147	template <class BidIt>
148	
149	inline bool prev_combination(BidIt n_begin, BidIt n_end,
150	BidIt r_begin, BidIt r_end)
151	{
152	  
153	  bool boolsame=false;
154	  BidIt marked;//for r
155	  BidIt r_marked;
156	  BidIt n_marked;
157	  
158	
159	  BidIt tmp_n_end=n_end;
160	  --tmp_n_end;
161	  
162	  BidIt r_it1=r_end;
163	  --r_it1;
164	  
165	  for(BidIt n_it1=tmp_n_end; n_it1!=n_begin || n_it1==n_begin ; --n_it1)
166	  {
167	    if(*r_it1==*n_it1)
168	    {
169	      r_marked=r_it1;
170	      n_marked=n_it1;
171	      break;
172	    }
173	  }  
174	  
175	  BidIt n_it2=n_marked;
176	
177	
178	  BidIt tmp_r_end=r_end;
179	  --tmp_r_end;
180	  
181	  for(BidIt r_it2=r_marked; r_it2!=r_begin || r_it2==r_begin; --r_it2,--n_it2)
182	  {
183	    if(*r_it2==*n_it2 )
184	    {
185	      if(r_it2==r_begin&& !(*r_it2==*n_begin) )
186	      {
187	        for(BidIt n_it3=n_begin;n_it3!=n_end;++n_it3)
188	        {
189	          if(*r_it2==*n_it3)
190	          {
191	            marked=r_it2;
192	            *r_it2=*(--n_it3);
193	            
194	            BidIt n_it4=n_end;
195	            --n_it4;
196	            for(BidIt r_it3=tmp_r_end; (r_it3!=r_begin || r_it3==r_begin) &&r_it3!=marked; --r_it3,--n_it4)
197	            {
198	              *r_it3=*n_it4;              
199	            }
200	            return true;
201	          }
202	        }
203	      }
204	      else if(r_it2==r_begin&&*r_it2==*n_begin)
205	      {
206	        return false;//no more previous combination; 
207	      }
208	    }
209	    else //if(*r_it2!=*n_it2 )
210	    {
211	      ++r_it2;
212	      marked=r_it2;
213	      for(BidIt n_it5=n_begin;n_it5!=n_end;++n_it5)
214	      {
215	        if(*r_it2==*n_it5)
216	        {
217	          *r_it2=*(--n_it5);
218	
219	          BidIt n_it6=n_end;
220	          --n_it6;
221	          for(BidIt r_it4=tmp_r_end; (r_it4!=r_begin || r_it4==r_begin) &&r_it4!=marked; --r_it4,--n_it6)
222	          {
223	            *r_it4=*n_it6;              
224	          }
225	          return true;
226	        }
227	      }
228	    }
229	  }  
230	  return false;//Will never reach here, unless error    
231	}
232	
233	
234	// Non recursive template function with Pred
235	template <class BidIt, class Prediate>
236	
237	inline bool prev_combination(
238	        BidIt n_begin, 
239	        BidIt n_end,
240	        BidIt r_begin, 
241	        BidIt r_end,
242	        Prediate Equal)
243	{
244	  
245	  bool boolsame=false;
246	  BidIt marked;//for r
247	  BidIt r_marked;
248	  BidIt n_marked;
249	  
250	
251	  BidIt tmp_n_end=n_end;
252	  --tmp_n_end;
253	  
254	  BidIt r_it1=r_end;
255	  --r_it1;
256	  
257	  for(BidIt n_it1=tmp_n_end; n_it1!=n_begin || n_it1==n_begin ; --n_it1)
258	  {
259	    if( Equal(*r_it1, *n_it1) )
260	    {
261	      r_marked=r_it1;
262	      n_marked=n_it1;
263	      break;
264	    }
265	  }  
266	  
267	  BidIt n_it2=n_marked;
268	
269	
270	  BidIt tmp_r_end=r_end;
271	  --tmp_r_end;
272	  
273	  for(BidIt r_it2=r_marked; r_it2!=r_begin || r_it2==r_begin; --r_it2,--n_it2)
274	  {
275	    if( Equal(*r_it2, *n_it2) )
276	    {
277	      if(r_it2==r_begin&& !Equal(*r_it2, *n_begin) )
278	      {
279	        for(BidIt n_it3=n_begin;n_it3!=n_end;++n_it3)
280	        {
281	          if(Equal(*r_it2, *n_it3))
282	          {
283	            marked=r_it2;
284	            *r_it2=*(--n_it3);
285	            
286	            BidIt n_it4=n_end;
287	            --n_it4;
288	            for(BidIt r_it3=tmp_r_end; (r_it3!=r_begin || r_it3==r_begin) &&r_it3!=marked; --r_it3,--n_it4)
289	            {
290	              *r_it3=*n_it4;              
291	            }
292	            return true;
293	          }
294	        }
295	      }
296	      else if(r_it2==r_begin&&Equal(*r_it2, *n_begin))
297	      {
298	        return false;//no more previous combination; 
299	      }
300	    }
301	    else //if(*r_it2!=*n_it2 )
302	    {
303	      ++r_it2;
304	      marked=r_it2;
305	      for(BidIt n_it5=n_begin;n_it5!=n_end;++n_it5)
306	      {
307	        if(Equal(*r_it2, *n_it5))
308	        {
309	          *r_it2=*(--n_it5);
310	
311	          BidIt n_it6=n_end;
312	          --n_it6;
313	          for(BidIt r_it4=tmp_r_end; (r_it4!=r_begin || r_it4==r_begin) &&r_it4!=marked; --r_it4,--n_it6)
314	          {
315	            *r_it4=*n_it6;              
316	          }
317	          return true;
318	        }
319	      }
320	    }
321	  }  
322	  return false;//Will never reach here, unless error    
323	}
324	
325	
326	// Recursive template function
327	template <class RanIt, class Func>
328	
329	void recursive_combination(RanIt nbegin, RanIt nend, int n_column,
330	                      RanIt rbegin, RanIt rend, int r_column,int loop, Func func)
331	{
332	        
333	        int r_size=rend-rbegin;
334	        
335	        
336	        int localloop=loop;
337	        int local_n_column=n_column;
338	        
339	        //A different combination is out
340	        if(r_column>(r_size-1))
341	        {
342	    func(rbegin,rend);
343	    return;
344	        }
345	        /////////////////////////////////
346	        
347	        for(int i=0;i<=loop;++i)
348	        {
349	                                
350	                RanIt it1=rbegin;
351	                for(int cnt=0;cnt<r_column;++cnt)
352	                {
353	                  ++it1;
354	                } 
355	                
356	                RanIt it2=nbegin;
357	                for(int cnt2=0;cnt2<n_column+i;++cnt2)
358	                {
359	                  ++it2;
360	                } 
361	                
362	                *it1=*it2;
363	                
364	                ++local_n_column;
365	                
366	                recursive_combination(nbegin,nend,local_n_column,
367	                        rbegin,rend,r_column+1,localloop,func);
368	                --localloop;
369	        }
370	        
371	}
372	
373	}
374	
375	#endif
