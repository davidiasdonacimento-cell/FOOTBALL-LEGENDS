# Gerar o APK Android

O repositório usa GitHub Actions e GameCI para compilar o projeto na nuvem. Não é necessário instalar o Unity Editor no celular.

## 1. Criar o arquivo de ativação Unity Personal

1. Abra a aba **Actions** do repositório.
2. Selecione **Request Unity Personal activation file**.
3. Toque em **Run workflow**.
4. Aguarde a execução terminar e baixe o artefato `Unity-activation-file`.
5. Extraia o arquivo `.alf` do ZIP.
6. Envie o `.alf` no portal oficial de ativação manual da Unity: https://license.unity3d.com/
7. Baixe o arquivo de licença `.ulf` que a Unity gerar.

## 2. Salvar a licença como Secret

No GitHub, abra **Settings → Secrets and variables → Actions → New repository secret**.

Crie estes secrets:

- `UNITY_LICENSE`: conteúdo completo do arquivo `.ulf`.
- `UNITY_EMAIL`: e-mail da conta Unity.
- `UNITY_PASSWORD`: senha da conta Unity.

Nunca coloque esses dados em arquivos do repositório, mensagens públicas ou código.

## 3. Gerar o APK

1. Abra **Actions**.
2. Selecione **Build Android APK**.
3. Toque em **Run workflow**.
4. Se a licença estiver válida, o job compilará o projeto.
5. Ao terminar, baixe o artefato **FOOTBALL-LEGENDS-Android-APK**.

## Observações

- A primeira compilação só poderá ocorrer depois de configurar os secrets.
- A licença Unity Personal é gratuita, sujeita aos termos e limites da Unity.
- O artefato de CI comprova que o pipeline gerou o APK; não comprova teste em aparelho real.
- O primeiro lobby usa formas provisórias de blockout. Os modelos finais e as animações serão integrados em etapas posteriores.
