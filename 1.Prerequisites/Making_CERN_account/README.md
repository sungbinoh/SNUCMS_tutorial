## CERN Account Registration

앞으로 CMS와 관련된 온라인 미팅 접속, 구성원과의 contact등을 위해서는 CERN account를 만드는 것은 필수적입니다.

우선 준비물은 다음과 같습니다 :

* 충분한 해상도의 여권 스캔본 (인적사항 있는 부분)
* CMS Secretariat과 주고받을수 있는 메일 (학교 메일을 안받기도 합니다)

### CERN 계정 만드는 방법 

회원가입하는 방법은 다음과 같습니다. 

1. 우선, CMS Secretariat에게 우선 다음과 같은 메일을 보내야합니다. (cms.people골뱅이cern.ch) 

   > Hello,
   > 
   > This is _이름_  from Seoul National University. \
   > I am trying to register as a CMS researcher, \
   > and I would like to know about the registration process.
   > 
   > Sincerely, _이름_\
   > Seoul National University 
 
2. 제네바 시간 기준 아침정도가 되면 (서울시간 기준 오후 4~5시 ) CMS Secretariat에게 다음과 같은 답장을 받을겁니다.
 
    > Good morning,
    > 
    > In order to get registered into CMS:
    >
    > STEP 1: \
    > Upload a copy (scan) of your passport or official identification document, using the link \
    > (the link is secure as it is using https)\
    > (use enough scan resolution in order to get it readable by the secretariat)\
    > _링크1_
    > 
    > STEP 2: \
    > Fill in the registration form, using the link\
    > _링크2_
    > 
    > The form will then be forwarded (for acknowledgement) to the team leader of your institute.\
    > (PLEASE NOTE THAT THOSE TWO LINKS SHOULD ONLY BE USED BY THE SAME PERSON.)
    > 
    > Best regards\
    > The CMS secretariat

3. 앞서 받은 메일의 _링크1_ 에 접속해, 준비한 여권 스캔본을 업로드해줍니다.
4. 이제 _링크2_ 에 접속해 registration form을 작성해줍니다.
   1. Member
       - Gender, Family name, First name, Nationality, Birth date : 인적사항대로 기재
       - Your main porject in CMS : Upgrade
      - Activity : Non Doctoral Student
   2. Institute
        - CMS Institute : Seoul National University 
        - Non CMS Institute : 기재하지 않고 다음 스텝으로 넘어갈 수 있습니다.
   3. CERN
       - 이 스텝은 스킵하셔도 좋습니다
   4. Interests
       - 관심 사항 체크하시면 되는데, 잘 모르겠으시면 Interested에 Physics 체크하시고 넘어가시면 됩니다.
   5. Submit

5. 교수님께 CMS 계정 생성 관련해서 메일을 확인해달라고 말씀드리고, 기다립니다.
6. 교수님께서 CMS측에 멤버 승인을 보내면, 다음과 같은 답장이 옵니다. 
    > 
    > Dear _이름_ (SEOUL-SNU), \
    > You are now registered in CMS as 'Non-Doctoral Student' from your institute (SEOUL-SNU) .
    > 
    > Family name:   \
    > First name:    \
    > HRpersonId:    
    >
    > IF you do not have a CERN computing account,\
    > a primary account and a CERN e-mail address will be created for you.\
    > To activate your account and obtain the login name and temporary password, please follow\
    > the procedure of  http://espace.cern.ch/identitymanagement/Public/NewComerFlyer.pdf
    >
    > IF this is your first CERN account,\
    > please contact 'cms.computing@cern.ch'  and give your account name\
    > OR\
    > go to https://account.cern.ch/account/ , then 'Services' tab, then 'Linux and AFS'\
    > subscribe and choose computing group 'ZH' for CMS, instead of the 'default',\
    > so that your account can be attached to the CMS group.
    >
    > IF and only if  you already have used a CERN account, please see\
    > choices  (A),  (B) and (C)  below.

    - 위 메일의 지시사항대로 아이디를 생성하면 됩니다. 아이디의 경우 CERN에서 이름따라 임의로 만들어줍니다. 
    - service-desk@cern.ch 에 다음과 같은 메일을 여권 사진을 별첨하여 보내줍니다. 
        - **임의로 부여받은 아이디가 조금이라도 이상한 경우 (가독성이 떨어지거나, 이름하고 많이 다른 경우) 꼭 service-desk@cern.ch에 아이디 변경 관련해서 메일로 요청을 하시길 바랍니다.** 

    >Good Morning, 
    >
    > This is _이름_  who just registered as a Non-Doctoral Student from Seoul-SNU.\
    > I would like to obtain my login name and the temporary password.\
    > A copy of my identification is attached.\ 
    >
    > Sincerely, _이름_ \
    > Seoul National University

7. 앞서 보낸 메일로 임시 비밀번호를 부여받으면, [Welcome Wizard](http://cern.ch/wizard)에 접속하여 지시대로 이행하여 계정 초기설정을 끝내줍니다. 
    - 임시 비밀번호를 부여받고나서 **최대 5일안에** 이 절차를 마무리해야합니다. 
    - Computing Rules : 기본적인 보안 상식을 물어보는 퀴즈를 풀고 다 맞아야 다음 스텝으로 넘어갑니다.
    - External Mail : 만일의 상황에 대비하여 외부 메일 연동을 해줍니다.
    - Mail Configuration : Cern Mailbox 또는 Extenal Mail 선택을 해줍니다.
    - Linux : zh : Linux group zh - CMS
    - CERN & EDH Password : 기준에 맞춰 비밀번호를 만들어줍니다.
8. 비밀번호, 아이디, 메일주소를 받고나면 CERN 계정 생성 완료

### lxplus 용량 늘리기 

- AFS 용량 늘리는 방법 : https://resources.web.cern.ch/resources/Manage/AFS/Settings.aspx 
  - lxplus 서버의 /afs/cern.ch/user/u/userloginname 디렉토리를 최대 10 GB 까지 늘릴 수 있습니다.
  - CERN 계정을 만들고나서 **무조건 늘리시길 바랍니다**.
  
