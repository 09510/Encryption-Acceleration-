#include<iostream> 
#include <vector> 
#include<time.h>
#include <cmath>
#include <algorithm>

#include <immintrin.h>

class Node
{
    public:
        Node()
        {
            label=-1;
        }

        Node * right_Node;
        Node * left_Node;
        int using_feature;
        double threshold;
        double impurity;
        int label;

        int test_val;
        int get_val();

};

class DecisionTree
{
    public:
        DecisionTree(std::string c,int  d )
        {
            max_depth=d;
            criterion=c;
        }
        std::string criterion;
        int max_depth;
        int total_feature;
        Node * root;
        double debug[10]={0,0,0,0,0,0,0,0,0,0};


        Node* split(std::vector<std::vector<double>>  data,int now_depth);
        void build_tree(std::vector<double> x,std::vector<double> y,int data_num);
        std::vector<int>  classfy(std::vector<double> x,std::vector<double> y,int data_num,bool for_forest);
        
        void get_one_node_feature();
        void fit();
        std::vector<std::vector<double>> set_data_type(std::vector<double>x,std::vector<double> y ,int feature_num)
        {
            total_feature = feature_num;
            int data_num = x.size()/total_feature;
            std::vector<std::vector<double>> after_shape;

            for(int i=0;i<data_num;i++)
            {
                std::vector<double> this_v;
                for(int f=0;f<total_feature;f++)
                {
                    this_v.push_back(x[f + i * total_feature]);
                }
                this_v.push_back(y[i]);
                after_shape.push_back(this_v);
            }

            return after_shape;
        }
        void debug_print(std::vector<std::vector<double>> t)
        {
            int row=t.size();
            int column=t[1].size();

            for(int i =0;i<row;i++)
            {
                std::cout<<i<<'\t';
                for(int j = 0;j<column;j++)
                {
                    std::cout<<t[i][j]<<" \t";
                }

                std::cout<<std::endl;
            }
        }

        void debug_v(std::vector<double> v)
        {
            for(int i=0;i<v.size();i++)
            {
                
                std::cout<<v[i]<<",";
            }
            std::cout<<std::endl;
        }
        void debug_tree_build(Node*n)
        {
            
            if(root->right_Node!=nullptr || root->left_Node!=nullptr)
            {
                
                std::cout<<"impurity:"<<n->impurity
                    <<"\tthreshold:"<<n->threshold
                    <<"\t"<<n->using_feature<<std::endl;
                
                if(n->right_Node!=nullptr){
                debug_tree_build(n->right_Node);
                }
                else{
                debug_tree_build(n->left_Node);
                }
            }
        }
        
        std::vector<double> transform(std::vector<std::vector<double>>x)
        {
            /*
            std::vector<std::vector<double>> ans(x[0].size(),std::vector<double>(x.size()));
            for(int i=0;i<x.size();i++)
            {
                for(int j=0;j<x[0].size();j++)
                {
                   ans[j][i]=x[i][j];
                }
            }
            return ans;
            */
            std::vector<double> ans;
            for(int i=0;i<x.size();i++)
            {
                ans.push_back(*(x[i].end()-1));
            }
            //debug_v(ans);
            //loat a=1/0;
            return ans;
        }
        int* p_transform(std::vector<std::vector<double>>x,int arr[])
        {
            for(int i=0;i<x.size();i++ )
            {
                arr[i]=x[i][x[0].size()-1];
            }
            return arr;
        }
/*        
        double v_gini(std::vector<double>::iterator start,std::vector<double>::iterator end,double length)
        {
            
            double num_one=0;
            for(std::vector<double>::iterator i = start; i!= end;++i)
            {
                num_one+=*i;
            }

            double purity=num_one/length;
            double g = pow(purity,2)+pow(1-purity,2);
            
            //std::cout<<g<<std::endl;
            g=1-g;
            return g;
        }
*/      
        int SumArray(int *buf,int N)
        {

            std::cout<<"N is :"<<N<<std::endl;
            int i;
            __m128i *vec128 = (__m128i *)buf;
            __m128i sum;
            
            std::cout<<2<<std::endl;
            sum = _mm_sub_epi32(sum,sum);
            for(i=0;i<N/4;i++ ){
                sum = _mm_add_epi32(sum,vec128[i]);

            std::cout<<2.5f<<std::endl;
            }
                sum = _mm_add_epi32(sum,_mm_srli_si128(sum,8));
                sum = _mm_add_epi32(sum,_mm_srli_si128(sum,4));
            

            std::cout<<3<<std::endl;

            return _mm_cvtsi128_si32(sum);
        }
        int32_t sumint_sse(const int32_t* pbuf, size_t cntbuf)
        {
            int32_t s = 0;	// 求和變量.
            size_t i;
            size_t nBlockWidth = 4;	// 塊寬. SSE寄存器能一次處理4個int32_t.
            size_t cntBlock = cntbuf / nBlockWidth;	// 塊數.
            size_t cntRem = cntbuf % nBlockWidth;	// 剩餘數量.
            __m128i xidSum = _mm_setzero_si128();	// 求和變量。[SSE2] PXOR. 賦初值0.
            __m128i xidLoad;	// 加載.
            const __m128i* p = (const __m128i*)pbuf;	// SSE批量處理時所用的指針.
            const int32_t* q;	// 單個數據處理時所用指針.

            // SSE批量處理.
            for(i=0; i<cntBlock; ++i)
            {
                //cout<<i<<endl;
                xidLoad = _mm_load_si128(p);	// [SSE2] MOVDQA. 加載.
                xidSum = _mm_add_epi32(xidSum, xidLoad);	// [SSE2] PADDD. 32位整數緊縮環繞加法.
                p ++;
            }
            // 合併.
            q = (const int32_t*)&xidSum;
            s = q[0] + q[1] + q[2] + q[3];

            // 處理剩下的.
            q = (const int32_t*)p;
            for(i=0; i<cntRem; ++i)
            {
                s += q[i];
            }

            return s;
        }
        double p_gini(int* p,int length)
        {
            //std::cout<<1<<std::endl;
            double num_one=0;
            //num_one=SumArray(p,length-1);

            //std::cout<<4<<std::endl;
            
            for(int i=0;i<length;i++)
            {
                num_one+=*(p+i);
            }
            

            double purity=num_one/length;
            double g = pow(purity,2)+pow(1-purity,2);
            
            //std::cout<<5<<std::endl;
            //std::cout<<g<<std::endl;
            g=1-g;
            return g;
        }

        double simd_gini(int* p,int length,int data_count,int total)
        {
            double num_one=0;
            
            for(int i=0;i<length;i++)
            {
                num_one+=*(p+i);
            }
            

            double purity=num_one/length;
            double g = pow(purity,2)+pow(1-purity,2);
            
            //std::cout<<5<<std::endl;
            //std::cout<<g<<std::endl;
            g=1-g;


            double right_purity=(total-num_one)/data_count;
            double right_g=pow(right_purity,2)+pow(1-right_purity,2);

            right_g=1-right_g;
            double total_impurity = length*g + (data_count-length)*right_g;
            total_impurity = total_impurity/data_count;
            return total_impurity;
        }

        std::vector<std::vector<double>> sort_and_get(std::vector<std::vector<double>>x,int f)
        {
            std::vector<std::vector<double>> ans(x.size(),std::vector<double>(2));
            for(int i=0;i<x.size();i++)
            {
                ans[i][0]=x[i][f];
                ans[i][1]=x[i][total_feature];
            }
            std::sort(ans.begin(),
            ans.end(),
            [f] (const std::vector<double> &a, const std::vector<double> &b)
            {
                return a[0] < b[0];
            });

/*
            for(int i=0;i<x.size();i++)
            {
                std::cout<<"f:"<<ans[i][0]<<"\ty:"<<ans[i][1]<<std::endl;
            }
            float s=2/0;
*/
            return ans;
        }

};

class RandomForest
{
    public:
        RandomForest()
        {
            DecisionTree * t;
            forest.push_back(t);
            max_depth=3;
        }
        std::vector<DecisionTree*> forest;
        int max_depth;
        int max_feature;
        std::string criterion;




        void build_forest();
        int test()
        {
            return max_depth;
        }

        
};


