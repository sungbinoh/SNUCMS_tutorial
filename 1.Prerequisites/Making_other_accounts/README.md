## Other Accounts Registration

CERN 계정을 만들게되면 lxplus 서버에 접속 권한을 얻게 됩니다. lxplus 이외에도 사용할 서버들이 있는데, 이 페이지에서는 해당 서버들에 계정을 만들고 접속 권한을 얻는 방법을 설명하도록 하겠습니다. 

- **앞으로 만들 계정들의 아이디는 꼭 CERN 계정 아이디와 동일하게 유지하시길 바랍니다.**
- **CERN 아이디 확정 받으시고 나서 (이상하면 바꾸신 뒤) 아래 계정들을 만들어주시길 바랍니다** 

### KNU 서버 계정 만들기

[KNU 서버 메뉴얼](http://t2-cms.knu.ac.kr/wiki/index.php/How_to_use_Tier2_at_KNU) 참고

1. [KNU 계정 신청 페이지](http://t2-cms.knu.ac.kr/lcg_registration.php)에 접속하여 필수 항목을 잔부 기재한뒤 제출해줍니다.
2. [사용계획서](http://t2-cms.knu.ac.kr/upload/Tier3_KR_KNU.hwp)를 작성하여 lcg_knu@knu.ac.kr 로 보내줍니다. 
3. 임시 비밀번호가 포함된 이메일을 받으면, 서버에 접속해서 비밀번호를 바꿔줍니다.

    ```
    ssh -Y <ID>@kcms-t2.knu.ac.kr
    passwd
    ```


### KISTI 서버 계정 만들기 

 [KISTI 서버 메뉴얼](https://gsdc-cms.gitbook.io/gsdc_cmst3/kisti-gsdc-cms-tier-3) 참고 

1. [신청서 양식](https://firebasestorage.googleapis.com/v0/b/gitbook-28427.appspot.com/o/assets%2F-LnFciAC6zRrj7ubxkYB%2F-M8SrctpWzo6T-L5x-lF%2F-M8SrhaRCESCuQJ4GPK1%2FGSDC-Account-Application_Form_2020.pdf?alt=media&token=9701842c-2276-4bd8-97e2-6751282ba206)을 다운로드 받고 인적사항을 채워줍니다. 
2. 교수님 서명을 받은 뒤, cmst3-support@kisti.re.kr로 신청서를 보냅니다.
3. KISTI-GSDC Service Team에서 임시 비밀번호가 포함된 이메일을 받게 됩니다. 
4. 서버에 접속해서 임시 비밀번호를 바꿔 줍니다. 

    ```
    ssh -Y -p 4280 <ID>@ui10.sdfarm.kr
    passwd
    ```

### Github Setup


1. [Github](https://github.com/join) 계정을 만들어줍니다.
2. lxplus, KISTI, KNU 등 git을 사용할 서버에 접속해주고 다음과 같이 초기 설정을 해줍니다.
   
   ```
   git config --global user.name "First name Last name"
   git config --global user.email <Your-Email-Address>
   git config --global user.github <Your-Just-Created-GitHub-Account-Username> 
   ```
3. ssh 키를 만들어주고, 계정에 등록해줍니다. ([링크 참조](https://docs.github.com/en/github/authenticating-to-github/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent))
