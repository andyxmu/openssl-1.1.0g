#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/ossl_typ.h>

void
main (int argc, char *argv[])
{
    EVP_MD_CTX mdctx;
    const EVP_MD *md;
    char mess2[] = "Hello World";
    char mess1[] = "Test Message";
    char sp1[] = "Hello ";
    char sp2[] = "World";
    unsigned char md_value[EVP_MAX_MD_SIZE];
    int md_len, i;

    //使EVP_Digest系列函数支持所有有效的信息摘要算法
    OpenSSL_add_all_digests();
    if(!argv[1]) {
        printf("Usage: mdtest digestname\n");
        exit(1);
    }
    //根据输入的信息摘要函数的名字得到相应的EVP_MD算法结构
    md = EVP_get_digestbyname(argv[1]);
    if(!md) {
        printf("Unknown message digest %s\n", argv[1]);
        exit(1);
    }
    //初始化信息摘要结构mdctx，这在调用EVP_DigestInit_ex函数的时候是必须的。

    EVP_MD_CTX_init(&mdctx);
    //使用md的算法结构设置mdctx结构，impl为NULL，即使用缺省实现的算法（openssl本身提供的信息摘要算法）
    EVP_DigestInit_ex(&mdctx, md, NULL);
    //开始真正进行信息摘要运算，可以多次调用该函数，处理更多的数据，这里只调用了两次
    //EVP_DigestUpdate(&mdctx, mess1, strlen(mess1));
    //EVP_DigestUpdate(&mdctx, mess2, strlen(mess2));
    EVP_DigestUpdate(&mdctx, sp1, strlen(sp1));
    EVP_DigestUpdate(&mdctx, sp2, strlen(sp2));
    //完成信息摘要计算过程，将完成的摘要信息存储在md_value里面,长度信息存储在md_len里面
    EVP_DigestFinal_ex(&mdctx, md_value, &md_len);
    //使用该函数释放mdctx占用的资源，如果使用_ex系列函数，这是必须调用的。
    EVP_MD_CTX_cleanup(&mdctx);
    printf("Digest is: ");
    for(i = 0; i < md_len; i++) printf("%02x", md_value[i]);
    printf("\n");

}
