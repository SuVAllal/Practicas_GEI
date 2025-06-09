from view import View
from model import Model
from presenter import Presenter
import os
import locale
import gettext
from pathlib import Path




# Determina la configuración local y el idioma del sistema
lang_code= locale.getdefaultlocale()[0]

locale_dir = Path(__file__).parent / "locales"

# Ruta donde estarán los archivos de traducción .mo
gettext.bindtextdomain('App', locale_dir)
gettext.textdomain('App')
lang_translations = gettext.translation('App', locale_dir, languages=[lang_code], fallback=True)
lang_translations.install()



if __name__ == "__main__":
    view = View(lang_translations)
    model = Model()

    presenter = Presenter(view, model)

    presenter.run(application_id="gal.udc.fic.ipm.PatientList")